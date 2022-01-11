// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTile.h"
#include "MyGameManager.h"

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
	
	CreateGameBoard(Rows, Cols);
}

// Called every frame
void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGameManager::CreateGameBoard(int i, int j)
{
	Rows = i;
	Cols = j;

	if (TileClass != nullptr)
	{
		
	}

	// Test
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 90.f, 0.f));
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(0.f, 100.f, 0.f), FRotator(0.f, 90.f, 0.f));
	GetWorld()->SpawnActor<AMyTile>(TileClass, FVector(-86.5f, 50.f, 0.f), FRotator(0.f, 90.f, 0.f));

}