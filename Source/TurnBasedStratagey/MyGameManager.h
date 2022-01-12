// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTile.h"
#include "MyGameManager.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGameManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyPathfindingManager* Pathfinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMyTile* TileRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* TileClass;

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

	UFUNCTION(BlueprintCallable)
		void CreateGameBoard(int i, int j);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
