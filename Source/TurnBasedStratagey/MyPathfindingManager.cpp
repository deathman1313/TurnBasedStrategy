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

void AMyPathfindingManager::Setup(TMap<FVector, AMyTile*> Tiles)
{
	Grid = Tiles;

	// Assign tile neighbors
	TArray<FVector> Keys;
	Keys.Empty();
	Grid.GenerateKeyArray(Keys);
	for (int i = 0; i < Keys.Num(); i++)
	{
		Grid[Keys[i]]->Neighbours = GetNeighbours(Keys[i]);
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

TArray<AMyTile*> AMyPathfindingManager::GetNeighbours(FVector TileKey)
{
	TArray<AMyTile*> Neighbors;
	// Check for neighbour on right
	if (Grid.Contains(TileKey + FVector(1, 0, -1)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(1, 0, -1)));
	}
	// Check for neighbour on bottom right
	if (Grid.Contains(TileKey + FVector(0, -1, -1)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(0, -1, -1)));
	}
	// Check for neighbour on bottom left
	if (Grid.Contains(TileKey + FVector(-1, -1, 0)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(-1, -1, 0)));
	}
	// Check for neighbour on left
	if (Grid.Contains(TileKey + FVector(-1, 0, 1)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(-1, 0, 1)));
	}
	// Check for neighbour on top left
	if (Grid.Contains(TileKey + FVector(0, 1, 1)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(0, 1, 1)));
	}
	// Check for neighbour on top right
	if (Grid.Contains(TileKey + FVector(1, 1, 0)))
	{
		Neighbors.Add(*Grid.Find(TileKey + FVector(1, 1, 0)));
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

TArray<AMyTile*> AMyPathfindingManager::ValidatePath(TArray<AMyTile*> Path, int UnitLayer = -1)
{
	bool bIsPathValid = true;
	for (AMyTile* Tile : Path)
	{
		// Check if tile is valid
		if (!Tile->bTraversable || IsTileOccupied(Tile, UnitLayer)) 
		{
			bIsPathValid = false;
			break;
		}
	}
	if (bIsPathValid)
	{
		return(Path);
	}
	else
	{
		return(FindPath(Path[0], Path[Path.Num() - 1], UnitLayer));
	}
}