// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseUnit.h"
#include "MyArcher.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API AMyArcher : public AMyBaseUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyArcher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
