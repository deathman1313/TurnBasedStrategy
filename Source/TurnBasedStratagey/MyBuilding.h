// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTurnObject.h"
#include "MyBuilding.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyBuilding : public AMyTurnObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBuilding();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> ValidTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Tile mesh it should be sitting on, leave nullptr if not required.
		UStaticMesh* TileBase;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DestroySelf() override;

	virtual void TurnAction() override;

	virtual void Reset() override;

};
