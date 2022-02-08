// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTile.h"
#include "MyGameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTryProgressTurn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStart);

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

UCLASS()
class TURNBASEDSTRATAGEY_API AMyGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGameManager();

	UPROPERTY(BlueprintAssignable)
		FOnTryProgressTurn OnTryProgressTurn;

	UPROPERTY(BlueprintAssignable)
		FOnRoundStart OnRoundStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* Selector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInstancedStaticMeshComponent* VisualPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bMidTurn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTurnObject*> TurnObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTurnObject*> WaitingFor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyPathfindingManager* Pathfinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMyTile* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Turn = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Rows = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Cols = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bGenerateTiles = true;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTopHeavy = true;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<AController*, FColor> PlayerColours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FConstructions> PossibleConstrutions;

	UFUNCTION(BlueprintCallable)
		void NextTurn();

	UFUNCTION()
		void CheckTurn(class AMyTurnObject* TurnObject);

	UFUNCTION(BlueprintCallable)
		void CreateGameBoard(int i, int j);

	UFUNCTION(BlueprintCallable)
		void CreateSelector(AMyTile* Tile);

	UFUNCTION(BlueprintCallable)
		void CreatePath(TArray<AMyTile*> Path);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
