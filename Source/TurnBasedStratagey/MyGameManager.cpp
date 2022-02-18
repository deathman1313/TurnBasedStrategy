// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameManager.h"
#include "MyTile.h"
#include "MyBase.h"
#include "MyPathfindingManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ChildActorComponent.h"

// Sets default values
AMyGameManager::AMyGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(ActorRoot);

	Selector = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Selector"));
	Selector->SetupAttachment(ActorRoot);

	VisualPath = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Path"));
	VisualPath->SetupAttachment(ActorRoot);

}

// Called when the game starts or when spawned
void AMyGameManager::BeginPlay()
{
	Super::BeginPlay();

	// Assign Player Colours
	PlayerColours.Add(GetWorld()->GetFirstPlayerController(), FColor::Blue);

	if (bGenerateTiles)
	{
		CreateGameBoard(Rows, Cols);
	}

	// Would make sense to have option here to locate tiles if none are provided

	// Show the mouse cursor
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	Pathfinding->Setup(Tiles);

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

void AMyGameManager::CreateGameBoard(int NewRows, int NewCols)
{
	// Make sure variables are valid
	Rows = (FMath::FloorToInt(NewRows * 0.5f) * 2) + 1;
	int HalfWidth = (Rows - 1) * 0.5f;
	Cols = (FMath::FloorToInt(NewCols * 0.5f) * 2) + 1;
	int HalfHeight = (Cols - 1) * 0.5f;

	if (TileClass)
	{
		for (int r = -HalfHeight; r <= HalfHeight; r++)
		{
			for (int q = -HalfWidth + FMath::CeilToInt(r * 0.5f); q <= (-HalfWidth + FMath::CeilToInt(r * 0.5f)) + (((Rows - 1) - (r % 2 != 0))); q++)
			{
				int s = -q - r;
				AMyTile* NewTile = GetWorld()->SpawnActor<AMyTile>(TileClass, GridToWorld(FVector2D(r, q)), FRotator(0.f, 90.f, 0.f));
				Tiles.Add(FVector(q, r, s), NewTile);
			}
		}
		TArray<FVector> TileKeys;
		TileKeys.Empty();
		Tiles.GenerateKeyArray(TileKeys);
		SpawnBase(Tiles[TileKeys[0]]);
		//SpawnBase(Tiles[TileKeys[TileKeys.Num() - 1]]);
	}
}

FVector AMyGameManager::GridToWorld(FVector2D GridLocation)
{
	FVector WorldLocation = GetActorLocation();
	WorldLocation.X = WorldLocation.X + (86.5f * GridLocation.X);
	WorldLocation.Y = WorldLocation.Y + (100 * GridLocation.Y) - (50 * GridLocation.X);
	return(WorldLocation);
}

void AMyGameManager::SpawnBase(AMyTile* Tile)
{
	if (Tile)
	{
		FActorSpawnParameters TempSpawnParams;
		TempSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMyBase* NewBase = GetWorld()->SpawnActor<AMyBase>(PossibleBuildings[1], Tile->GetActorLocation() + Tile->TileCenter->GetComponentLocation(), FRotator(0.f, 90.f, 0.f), TempSpawnParams);
		if (NewBase)
		{
			NewBase->OnTile = Tile;
			Tile->Building = NewBase;
		}
	}
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