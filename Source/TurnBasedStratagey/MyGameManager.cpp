// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameManager.h"
#include "MyGameInstance.h"
#include "MyTile.h"
#include "MyBase.h"
#include "MyMountain.h"
#include "MyAIPlayerController.h"
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

	// Assign Players
	UMyGameInstance* GInst = Cast<UMyGameInstance>(GetGameInstance());
	FPlayerInfo NewPlayer;
	if (GInst)
	{
		// For testing/bugcatching, not required
		if (GInst->PlayersAreHuman.Num() < 2)
		{
			GInst->PlayersAreHuman.Add(true);
			GInst->PlayersAreHuman.Add(false);
			GInst->PlayersAreHuman.Add(false);
			GInst->PlayerColours.Add(FColor::Blue);
			GInst->PlayerColours.Add(FColor::Red);
			GInst->PlayerColours.Add(FColor::Green);
			GInst->PlayerNames.Add("You");
			GInst->PlayerNames.Add("AI1");
			GInst->PlayerNames.Add("AI2");
		}
		for (int i = 0; i < GInst->PlayersAreHuman.Num(); i++)
		{
			// Set/create controller
			if (GInst->PlayersAreHuman[i])
			{
				NewPlayer.PlayerController = GetWorld()->GetFirstPlayerController();
			}
			else
			{
				NewPlayer.PlayerController = SpawnAI(Players.Num());
			}
			// Assign player colour
			NewPlayer.PlayerColour = GInst->PlayerColours[i];
			// Assign player name
			if (GInst->PlayerNames[i] != "")
			{
				NewPlayer.PlayerName = GInst->PlayerNames[i];
			}
			else
			{
				NewPlayer.PlayerName = "Player " + FString::FromInt(i + 1);
			}
			Players.Add(NewPlayer);
		}
		// Inherit grid size
		Rows = GInst->Rows;
		Cols = GInst->Cols;
	}
	// Add new event dispatchers
	for (FPlayerInfo Player : Players)
	{
		FOnTryProgressTurn TempProgressTurn;
		OnTryProgressTurn.Add(TempProgressTurn);
		FOnRoundStart TempRoundStart;
		OnRoundStart.Add(TempRoundStart);
		AMyAIPlayerController* AIController = Cast<AMyAIPlayerController>(Player.PlayerController);
		if (AIController)
		{
			// Assign event dispatcher
			OnRoundStart.Last().AddUniqueDynamic(AIController, &AMyAIPlayerController::StartTurn);
		}
	}
	// Create board
	if (bGenerateTiles)
	{
		CreateGameBoard(Rows, Cols);
	}

	// Would make sense to have option here to locate tiles if none are provided or generated

	// Show the mouse cursor/set input mode
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	FInputModeGameAndUI InputModeStruct;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputModeStruct);

	Pathfinding->Setup(Tiles);

	// Start first round
	OnRoundStart[ActivePlayer].Broadcast();
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
	WaitingFor = Players[ActivePlayer].OwningBases;
	WaitingFor.Append(Players[ActivePlayer].OwningBuildings);
	WaitingFor.Append(Players[ActivePlayer].OwningUnits);
	OnTryProgressTurn[ActivePlayer].Broadcast();
}

void AMyGameManager::CheckTurn(AMyTurnObject* TurnObject)
{
	// Change to take into account unit movement, easier that changing AI
	// Check if all objects are ready to start next turn
	WaitingFor.Remove(TurnObject);
	if (WaitingFor.Num() <= 0)
	{
		// Start Next Turn
		ActivePlayer++;
		if (ActivePlayer >= Players.Num())
		{
			ActivePlayer = 0;
			Turn++;
		}
		if (Turn >= TurnLimit && TurnLimit != -1)
		{
			EndGame();
		}
		else
		{
			bMidTurn = false;
			UE_LOG(LogTemp, Warning, TEXT("StartNextTurn"));
			OnRoundStart[ActivePlayer].Broadcast();
		}
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
				int s = r - q;
				AMyTile* NewTile = GetWorld()->SpawnActor<AMyTile>(TileClass, GridToWorld(FVector2D(r, q)), FRotator(0.f, 90.f, 0.f));
				Tiles.Add(FVector(q, r, s), NewTile);
			}
		}
		// Spawn player bases
		for (int PlayerIndex = 0; PlayerIndex < Players.Num(); PlayerIndex++)
		{
			SpawnBase(FindEmptyTile(), PlayerIndex);
		}
		// Spawn enemy bases
		for (int i = 0; i < GenerationSettings.EmptyBaseNum; i++)
		{
			SpawnBase(FindEmptyTile(), -1);
		}
		// Spawn mountains
		for (int i = 0; i < GenerationSettings.MountainNum; i++)
		{
			SpawnMountain(FindEmptyTile());
		}
	}
}

FVector AMyGameManager::GridToWorld(FVector2D GridLocation)
{
	FVector WorldLocation = GetActorLocation();
	WorldLocation.X = WorldLocation.X + (86.5f * GridLocation.X);
	WorldLocation.Y = WorldLocation.Y + (100 * GridLocation.Y) - (50 * GridLocation.X);
	return(WorldLocation);
}

AMyTile* AMyGameManager::FindEmptyTile()
{
	TArray<FVector> TileKeys;
	TileKeys.Empty();
	Tiles.GenerateKeyArray(TileKeys);

	int RandTileIndex;
	for (int Err = 0; Err < 50; Err++)
	{
		RandTileIndex = FMath::RandRange(0, TileKeys.Num() - 1);
		if (!Tiles[TileKeys[RandTileIndex]]->Building)
		{
			// Should check for valid paths here
			return(Tiles[TileKeys[RandTileIndex]]);
		}
	}
	return(nullptr);
}

AMyAIPlayerController* AMyGameManager::SpawnAI(int PlayerID)
{
	// Spawn AI
	AMyAIPlayerController* NewAI = GetWorld()->SpawnActor<AMyAIPlayerController>();
	NewAI->PlayerIndex = PlayerID;
	return(NewAI);
}

void AMyGameManager::SpawnBase(AMyTile* Tile, int PlayerIndex)
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
			NewBase->UpdateOwner(PlayerIndex);
			NewBase->Setup();
		}
	}
}

void AMyGameManager::SpawnMountain(AMyTile* Tile)
{
	if (Tile)
	{
		FActorSpawnParameters TempSpawnParams;
		TempSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMyMountain* NewMountain = GetWorld()->SpawnActor<AMyMountain>(PossibleBuildings[0], Tile->GetActorLocation() + Tile->TileCenter->GetComponentLocation(), FRotator(0.f, 90.f, 0.f), TempSpawnParams);
		if (NewMountain)
		{
			NewMountain->OnTile = Tile;
			Tile->Building = NewMountain;
			Tile->bTraversable = false;
			NewMountain->Setup();
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

void AMyGameManager::EndGame()
{
	// Best option, doesn't work with -1 player
	TArray<int> Winners;
	int MostBases = 0;
	for (int i = 0; i < Players.Num(); i++)
	{
		if (Players[i].OwningBases.Num() > MostBases)
		{
			MostBases = Players[i].OwningBases.Num();
			Winners.Empty();
			Winners.Add(i);
		}
		else if (Players[i].OwningBases.Num() == MostBases)
		{
			Winners.Add(i);
		}
	}
	// These are the winners
	TArray<AController*> WinnerControllers;
	for (int PlayerIndex : Winners)
	{
		if (PlayerIndex > -1 && PlayerIndex < Players.Num())
		{
			WinnerControllers.Add(Players[PlayerIndex].PlayerController);
		}
	}
	OnGameEnd.Broadcast(WinnerControllers);
}