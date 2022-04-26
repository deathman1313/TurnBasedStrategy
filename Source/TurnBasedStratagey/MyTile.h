// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBuilding.h"
#include "MyTile.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USceneComponent* TileCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TArray<class UStaticMesh*> BaseTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TArray<class UMaterialInterface*> DefaultMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UMaterialInterface* SelectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
		bool bTraversable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
		float MoveCost = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
		class AMyBaseUnit* OccupyingUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
		class AMyBuilding* Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		// Row index
		int I;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		// Collumn index
		int J;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		AMyTile* PreviousTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		TArray<AMyTile*> Neighbours;

	UFUNCTION(BlueprintCallable)
		void SelectTile(bool Select);

	UFUNCTION(BlueprintCallable)
		bool TakeBase(int RivalPlayerIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
