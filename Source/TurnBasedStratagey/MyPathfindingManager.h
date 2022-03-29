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
		TMap<FVector, AMyTile*> Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Rows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Cols;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TilesInRow;

	UFUNCTION(BlueprintCallable)
		void Setup(TMap<FVector, AMyTile*> Tiles);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> FindPath(AMyTile* Start, AMyTile* End, int UnitLayer = -1, int OwningPlayerIndex = -1);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> GetNeighbours(FVector TileKey);

	UFUNCTION(BlueprintCallable)
		float GetTileDistance(AMyTile* Start, AMyTile* End);

	UFUNCTION(BlueprintCallable)
		bool IsTileOccupied(AMyTile* Tile, int UnitLayer);

	UFUNCTION(BlueprintCallable)
		bool IsBlockingBase(AMyTile* Tile, int OwningPlayerIndex);

	UFUNCTION(BlueprintCallable)
		TArray<AMyTile*> ValidatePath(TArray<AMyTile*> Path, int UnitLayer, int OwningPlayerIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
