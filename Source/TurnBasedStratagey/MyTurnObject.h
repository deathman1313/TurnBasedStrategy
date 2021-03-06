// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyTurnObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishAction, AMyTurnObject*, ActorRef);

UCLASS()
class TURNBASEDSTRATAGEY_API AMyTurnObject : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTurnObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyGameManager* GManager;

	UPROPERTY(BlueprintAssignable)
		FOnFinishAction OnFinishAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPerformedAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name = "Turn Object";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ObjectType = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Range = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AttackDamage = 10;

	// Probably unnecessary
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyTile* Targeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMyTile*> ValidTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int OwningPlayerIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyTile* OnTile;

	UFUNCTION(BlueprintCallable)
		virtual void Setup();

	UFUNCTION(BlueprintCallable)
		void ApplyDamage(int Damage);

	UFUNCTION(BlueprintCallable)
		virtual void DestroySelf();

	UFUNCTION(BlueprintCallable)
		virtual void TurnAction();

	UFUNCTION(BlueprintCallable)
		virtual void Reset();

	UFUNCTION(BlueprintCallable)
		void DoNothing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
