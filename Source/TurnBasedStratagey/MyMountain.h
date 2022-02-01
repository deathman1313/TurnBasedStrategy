// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBuilding.h"
#include "MyMountain.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API AMyMountain : public AMyBuilding
{
	GENERATED_BODY()
	
public:
	AMyMountain();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void TurnAction() override;

	virtual void Reset() override;
};
