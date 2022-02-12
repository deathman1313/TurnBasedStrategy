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

	//Pathfinding->Setup(Tiles, Rows, Cols);

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

void AMyGameManager::CreateGameBoard(int HalfWidth, int HalfHeight)
{
	// WIP
	Rows = (HalfWidth * 2) + 1;
	Cols = (HalfHeight * 2) + 1;

	if (TileClass)
	{
		for (int q = -HalfHeight; q <= HalfHeight; q++)
		{
			for (int r = -HalfWidth; r <= HalfWidth; r++)
			{
				int s = -q - r;
				AMyTile* NewTile = GetWorld()->SpawnActor<AMyTile>(TileClass, GridToWorld(FVector2D(q, r)), FRotator(0.f, 90.f, 0.f));
				Tiles.Add(FVector(q, r, s), NewTile);
			}
		}
	}
}

FVector AMyGameManager::GridToWorld(FVector2D GridLocation)
{
	FVector WorldLocation = GetActorLocation();
	WorldLocation.X = WorldLocation.X + (100 * (FMath::Sqrt(3) * GridLocation.X + FMath::Sqrt(3) * 0.5 * GridLocation.Y));
	WorldLocation.Y = WorldLocation.Y + (43.25f * (3 * 0.5 * GridLocation.Y));
	//WorldLocation.X = WorldLocation.X + (GridLocation.X * 100);
	//WorldLocation.Y = WorldLocation.Y + (GridLocation.Y * 100);
	return(WorldLocation);
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