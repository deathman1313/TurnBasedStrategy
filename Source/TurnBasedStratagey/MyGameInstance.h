// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameManager.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Cols = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Rows = 11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGenerationSettings GenerationSettings;
};
