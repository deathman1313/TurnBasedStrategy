// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTurnObject.h"
#include "MyAIPlayerController.h"
#include "MyBaseUnit.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyBaseUnit : public AMyTurnObject
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBaseUnit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* UIInfoLocation;

	// For AI controlled units only
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAIBehaviourTypes AIBehaviour;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int UnitLayer = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int MaxMovement = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int CurrentMovement = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TArray<class AMyTile*> MovementQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector MovementLocation;

	UFUNCTION(BlueprintCallable)
		bool ProcessMovement(bool bFirstPass = true);

	UFUNCTION(BlueprintCallable)
		TArray<class AMyTile*> FindTargets();

	UFUNCTION(BlueprintCallable)
		bool AttackTarget(AMyTile* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void DestroySelf() override;

	virtual void TurnAction() override;

	virtual void Reset() override;
};
