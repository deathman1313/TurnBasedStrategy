// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTurnObject.h"
#include "MyGameManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyTurnObject::AMyTurnObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTurnObject::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind listener to game manager start round
	GManager = Cast<AMyGameManager>(UGameplayStatics::GetActorOfClass(this, AMyGameManager::StaticClass()));
	GManager->OnTryProgressTurn.AddUniqueDynamic(this, &AMyTurnObject::TurnAction);
	GManager->OnRoundStart.AddUniqueDynamic(this, &AMyTurnObject::Reset);
	GManager->TurnObjects.Add(this);

	// Bind listeners in game manager
	OnFinishAction.AddDynamic(GManager, &AMyGameManager::CheckTurn);
}

// Called every frame
void AMyTurnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTurnObject::TurnAction()
{
	// Override this
	// Do action
	// Finished TurnAction
	if (bPerformedAction)
	{
		bLocked = true;
		OnFinishAction.Broadcast(this);
	}
}

void AMyTurnObject::Reset()
{
	// Override this
	// TriggerReset
	bPerformedAction = false;
	bLocked = false;
}

void AMyTurnObject::DoNothing()
{
	bPerformedAction = true;
}