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
	TArray<AMyTile*> test;
	return(test);
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

int AMyPathfindingManager::FindArrayIndex(int i, int j)
{
	return((((Cols * 2) - 1) * i) + j);
}