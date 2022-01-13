// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPathfindingManager.h"
#include "MyGameManager.h"

// Sets default values
AMyPathfindingManager::AMyPathfindingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

TArray<AMyTile*> AMyPathfindingManager::FindPath(AMyTile* Start, AMyTile* End)
{
	TArray<AMyTile*> OpenSet;
	TArray<AMyTile*> ClosedSet;

	OpenSet.Push(Start);

	//Check to make sure positions are valid, this may not be necessary
	if (Start && End)
	{
		// The currently selected tile to check against
		AMyTile* Current;
		while (OpenSet.Num() > 0)
		{
			Current = OpenSet[0];
			for (AMyTile* Tile : OpenSet)
			{
				if (Tile->F < Current->F)
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
					while (Current->PreviousTile)
					{
						Path.Push(Current->PreviousTile);
						Current = Current->PreviousTile;
					}
					return(Path);
				}
			}

			OpenSet.Remove(Current);
			ClosedSet.Push(Current);

			for (AMyTile* Neighbour : Current->Neighbours)
			{
				if (!ClosedSet.Contains(Neighbour))
				{
					// This might be where cost is calculated?
					int TempG = Current->G + 1;

					if (OpenSet.Contains(Neighbour))
					{
						// Update G
						if (TempG < Neighbour->G)
						{
							Neighbour->G = TempG;
						}
					}
					else
					{
						Neighbour->G = TempG;
						OpenSet.Push(Neighbour);
					}

					// Update H
					Neighbour->H = GetTileDistance(Neighbour, End);
					// Update F
					Neighbour->F = Neighbour->G + Neighbour->H;
					// Update Previous
					Neighbour->PreviousTile = Current;
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
	float xDif = FMath::Abs((Start->J - End->J) * 0.5f);
	int yDif = FMath::Abs(Start->I - End->I);
	return(xDif + yDif);
}

int AMyPathfindingManager::FindArrayIndex(int i, int j)
{
	return((((Cols * 2) - 1) * i) + j);
}