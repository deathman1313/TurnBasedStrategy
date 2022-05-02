// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIPlayerController.generated.h"

// WIP
UENUM(BlueprintType)
enum class EAIBehaviourTypes : uint8
{
	Default
};

USTRUCT(BlueprintType)
struct FAIBehaviourTraits
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAIBehaviourTypes Behaviour;
};

UCLASS()
class TURNBASEDSTRATAGEY_API AMyAIPlayerController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PlayerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTurnObject*> OwningObjects;

	UFUNCTION(BlueprintCallable)
		void GameEnded(TArray<AController*> WinnerControllers);

	UFUNCTION(BlueprintCallable)
		void StartTurn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaTime) override;
};
