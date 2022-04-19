// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> PlayerNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> PlayersAreHuman;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FColor> PlayerColours;
};
