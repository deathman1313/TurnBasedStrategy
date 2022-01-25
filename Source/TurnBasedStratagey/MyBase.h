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
		class AMyGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentConstruction = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ConstructionProgress = 0;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
