// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameManager.h"
#include "MyTile.h"
#include "MyPathfindingManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyGameManager::AMyGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Selector = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Selector"));
	Selector->SetupAttachment(GetRootComponent());

	VisualPath = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Path"));
	VisualPath->SetupAttachment(GetRootComponent());

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

	// Show the mouse cursor
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	Pathfinding->Setup(Tiles, Rows, Cols);

	// Assign Player Colours
	PlayerColours.Add(GetWorld()->GetFirstPlayerController(), FColor::Blue);
}

// Called every frame
void AMyGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGameManager::NextTurn()
{
	// Try to progress to new turn
	bMidTurn = true;
	WaitingFor = TurnObjects;
	OnTryProgressTurn.Broadcast();
}

void AMyGameManager::CheckTurn(AMyTurnObject* TurnObject)
{
	// Check if all objects are ready to start next turn
	WaitingFor.Remove(TurnObject);
	if (WaitingFor.Num() <= 0)
	{
		// Start Next Turn
		Turn++;
		bMidTurn = false;
		UE_LOG(LogTemp, Warning, TEXT("StartNextTurn"));
		OnRoundStart.Broadcast();
	}
}

void AMyGameManager::CreateGameBoard(int c, int r)
{
	// WIP
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

void AMyGameManager::CreateSelector(AMyTile* Tile)
{
	// Remove all existing selectors
	Selector->ClearInstances();
	if (Tile)
	{
		// Create at given location
		Selector->AddInstance(FTransform(FRotator(0.f, 0.f, 0.f), Tile->GetActorLocation() + Tile->TileCenter->GetComponentLocation()));
	}
}

void AMyGameManager::CreatePath(TArray<AMyTile*> Path)
{
	// Remove all existing paths
	VisualPath->ClearInstances();
	if (Path.Num() > 1)
	{
		// For all tiles in array, except the last
		for (int i = 0; i < Path.Num() - 1; i++)
		{
			// Find rotation between this and next tile
			FRotator ArrowRotation = (Path[i]->GetActorLocation() - Path[i+1]->GetActorLocation()).Rotation() + FRotator(0.f, 90.f, 0.f);
			// Create at given location, with calculated rotation
			VisualPath->AddInstance(FTransform(ArrowRotation, Path[i]->GetActorLocation() + Path[i]->TileCenter->GetComponentLocation()));
		}
	}
}