// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTile.h"
#include "MyPathfindingManager.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyPathfindingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPathfindingManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMyTile*> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Cols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TilesInRow;

	UFUNCTION(BlueprintCallable)
		//Converts 2D array index into correct 1D array index. Column position is spaced with empty indexs, to find correct j index double it and subtract 1 if it is on an odd row
		int FindArrayIndex(int i, int j);

	UFUNCTION(BlueprintCallable)
		void Setup(TArray<AMyTile*> Tiles, int R, int C);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> FindPath(AMyTile* Start, AMyTile* End, int UnitLayer);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> GetNeighbours(AMyTile* Tile);

	UFUNCTION(BlueprintCallable)
		float GetTileDistance(AMyTile* Start, AMyTile* End);

	UFUNCTION(BlueprintCallable)
		bool IsTileOccupied(AMyTile* Tile, int UnitLayer);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> ValidatePath(TArray<AMyTile*> Path, int UnitLayer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
