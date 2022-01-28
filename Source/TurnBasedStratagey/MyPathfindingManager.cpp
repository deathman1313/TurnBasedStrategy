// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPathfindingManager.h"
#include "MyGameManager.h"
#include "MyBaseUnit.h"
#include "Algo/Reverse.h"

// Sets default values
AMyPathfindingManager::AMyPathfindingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPathfindingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPathfindingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPathfindingManager::Setup(TArray<AMyTile*> Tiles, int R, int C)
{
	Rows = R;
	Cols = C;
	TilesInRow = (Cols * 2) - 1;
	// Add all tiles to spaced array
	for (int i = 0; i < Tiles.Num(); i++)
	{
		int NewPos = i * 2;

		Tiles[i]->I = FMath::FloorToInt(NewPos / TilesInRow);
		Tiles[i]->J = FMath::FloorToInt(NewPos % TilesInRow);

		Grid.Add(Tiles[i]);
		Grid.Add(nullptr);
	}
	Grid.Pop();

	// Assign tile neighbors
	for (int i = 0; i < Grid.Num() - 1; i = i + 2)
	{
		Grid[i]->Neighbours = GetNeighbours(Grid[i]);
	}
}

TArray<AMyTile*> AMyPathfindingManager::FindPath(AMyTile* Start, AMyTile* End, int UnitLayer = -1)
{
	TArray<AMyTile*> OpenSet;

	TMap<AMyTile*, float> G;
	TMap<AMyTile*, float> F;

	OpenSet.Push(Start);
	G.Add(Start, 0);
	F.Add(Start, 0);

	Start->PreviousTile = nullptr;

	if (Start->bTraversable && End->bTraversable && !IsTileOccupied(End, UnitLayer))
	{
		// The currently selected tile to check against
		AMyTile* Current;
		while (OpenSet.Num() > 0)
		{
			Current = OpenSet[0];
			for (AMyTile* Tile : OpenSet)
			{
				if (F[Tile] < F[Current])
				{
					Current = Tile;
				}
			}

			if (Current)
			{
				if (Current == End)
				{
					TArray<AMyTile*> Path;
					Path.Push(Current);
					// This should probably be changed to a map
					while (Current->PreviousTile)
					{
						Path.Push(Current->PreviousTile);
						Current = Current->PreviousTile;
					}
					Algo::Reverse(Path);
					return(Path);
				}
			}

			OpenSet.Remove(Current);

			for (AMyTile* Neighbour : Current->Neighbours)
			{
				if (Neighbour->bTraversable && !IsTileOccupied(Neighbour, UnitLayer))
				{
					float TempG = G[Current] + Neighbour->MoveCost;

					if (TempG < G.FindOrAdd(Neighbour, INFINITY))
					{
						Neighbour->PreviousTile = Current;
						G[Neighbour] = TempG;
						F.Add(Neighbour, TempG + GetTileDistance(Neighbour, End));
						if (!OpenSet.Contains(Neighbour))
						{
							OpenSet.Add(Neighbour);
						}
					}
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed Pathfind"));
	TArray<AMyTile*> InvalidArray;
	return(InvalidArray);
}

TArray<AMyTile*> AMyPathfindingManager::GetNeighbours(AMyTile* Tile)
{
	TArray<AMyTile*> Neighbors;
	// Check for neighbour on right
	if (Tile->J + 2 < TilesInRow)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I, Tile->J + 2)]);
	}
	// Check for neighbour on bottom right
	if (Tile->I + 1 < Rows && Tile->J + 1 < TilesInRow)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I + 1, Tile->J + 1)]);
	}
	// Check for neighbour on bottom left
	if (Tile->I + 1 < Rows && Tile->J - 1 >= 0)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I + 1, Tile->J - 1)]);
	}
	// Check for neighbour on left
	if (Tile->J - 2 >= 0)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I, Tile->J - 2)]);
	}
	// Check for neighbour on top left
	if (Tile->I - 1 >= 0 && Tile->J - 1 >= 0)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I - 1, Tile->J - 1)]);
	}
	// Check for neighbour on top right
	if (Tile->I - 1 >= 0 && Tile->J + 1 < TilesInRow)
	{
		Neighbors.Push(Grid[FindArrayIndex(Tile->I - 1, Tile->J + 1)]);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Neighbors.Num());
	return(Neighbors);
}

float AMyPathfindingManager::GetTileDistance(AMyTile* Start, AMyTile* End)
{
	// (Assume all roads and no blocks)
	int yDif = FMath::Abs(Start->I - End->I);
	float xDif = FMath::Abs(((Start->J - End->J) * 0.5f) - (yDif * 0.5f));
	return((xDif + yDif) * 0.5);
}

int AMyPathfindingManager::FindArrayIndex(int i, int j)
{
	return((((Cols * 2) - 1) * i) + j);
}

bool AMyPathfindingManager::IsTileOccupied(AMyTile* Tile, int UnitLayer)
{
	if (Tile->OccupyingUnit)
	{
		if (UnitLayer == Tile->OccupyingUnit->UnitLayer && UnitLayer != -1)
		{
			return(true);
		}
	}
	return(false);
}