// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameManager.h"
#include "MyTile.h"
#include "MyPathfindingManager.h"

// Sets default values
AMyGameManager::AMyGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (bGenerateTiles)
	{
		CreateGameBoard(Rows, Cols);
	}
	// Would make sense to have option here to locate tiles if none are provided

	Pathfinding->Setup(Tiles, Rows, Cols);
}

// Called every frame
void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGameManager::CreateGameBoard(int c, int r)
{
	Rows = c;
	Cols = r;

	if (TileClass)
	{
		//float x;
		//float y;
		for (int i = 0; i < Rows; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Row"));
			for (int j = 0; j < Cols; j++)
			{
				//AMyTile* newTile = GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(x, y, 20.f), FRotator(0.f, 90.f, 0.f));
				//Tiles.Push(newTile);
			}
		}
	}

	// Test
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(0.f, 0.f, 20.f), FRotator(0.f, 90.f, 0.f));
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(0.f, 100.f, 20.f), FRotator(0.f, 90.f, 0.f));
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(-86.5f, 50.f, 20.f), FRotator(0.f, 90.f, 0.f));

}