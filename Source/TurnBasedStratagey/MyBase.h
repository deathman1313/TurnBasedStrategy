// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBuilding.h"
#include "MyBase.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API AMyBase : public AMyBuilding
{
	GENERATED_BODY()

public:
	AMyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentConstruction = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ConstructionProgress = 0;

	UFUNCTION(BlueprintCallable)
		void UpdateOwner(int NewOwnerIndex);

	UFUNCTION(BlueprintCallable)
		bool SetConstruction(int NewConstruction);

	UFUNCTION(BlueprintCallable)
		bool SpawnUnit();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void DestroySelf() override;

	virtual void TurnAction() override;

	virtual void Reset() override;
};
