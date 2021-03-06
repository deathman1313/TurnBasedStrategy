// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPathfindingManager.h"
#include "MyTile.h"
#include "MyBaseUnit.h"
#include "MyGameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTryProgressTurn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd, TArray<AController*>, WinnerControllers);

USTRUCT(BlueprintType)
struct FConstructions
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AMyBaseUnit> Unit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ConstructionCost;
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FColor PlayerColour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTurnObject*> OwningUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTurnObject*> OwningBases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTurnObject*> OwningBuildings;
};

USTRUCT(BlueprintType)
struct FGenerationSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MountainNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int EmptyBaseNum;

	// No longer used by the code
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int HumanNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AINum;
};

UCLASS()
class TURNBASEDSTRATAGEY_API AMyGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGameManager();

	UPROPERTY(BlueprintAssignable)
		TArray<FOnTryProgressTurn> OnTryProgressTurn;

	UPROPERTY(BlueprintAssignable)
		TArray<FOnRoundStart> OnRoundStart;

	UPROPERTY(BlueprintAssignable)
		FOnGameEnd OnGameEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* ActorRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* Selector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* VisualPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bMidTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TurnLimit = 15;

	// Probably could be made unnecessary
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTurnObject*> TurnObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTurnObject*> WaitingFor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerInfo> Players;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMyPathfindingManager* Pathfinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMyTile* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Turn = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ActivePlayer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Rows = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Cols = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bGenerateTiles = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGenerationSettings GenerationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FConstructions> PossibleConstrutions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AActor>> PossibleBuildings;

	UFUNCTION(BlueprintCallable)
		void NextTurn();

	UFUNCTION(BlueprintCallable)
		void CheckTurn(class AMyTurnObject* TurnObject);

	UFUNCTION(BlueprintCallable)
		void CreateGameBoard(int NewRows, int NewCols);

	UFUNCTION(BlueprintCallable)
		FVector GridToWorld(FVector2D GridLocation);

	UFUNCTION(BlueprintCallable)
		AMyTile* FindTileFromLocation(FVector Location); 
	
	UFUNCTION(BlueprintCallable)
		void CreateSelector(AMyTile* Tile);

	UFUNCTION(BlueprintCallable)
		void CreatePath(TArray<AMyTile*> Path);

	UFUNCTION(BlueprintCallable)
		AMyTile* FindEmptyTile();

	UFUNCTION(BlueprintCallable)
		class AMyAIPlayerController* SpawnAI(int PlayerID);

	UFUNCTION(BlueprintCallable)
		void SpawnBase(AMyTile* Tile, int PlayerIndex);

	UFUNCTION(BlueprintCallable)
		void SpawnMountain(AMyTile* Tile);

	UFUNCTION(BlueprintCallable)
		void EndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
