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

	UPROPERTY(BlueprintAssignable)
		FOnFinishAction OnFinishAction;

	UFUNCTION(BlueprintCallable)
		void TurnAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
