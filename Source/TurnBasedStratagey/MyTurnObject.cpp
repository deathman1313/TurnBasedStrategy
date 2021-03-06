// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTurnObject.h"
#include "Components/SceneComponent.h"
#include "MyGameManager.h"
#include "MyBase.h"
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
}

// Called every frame
void AMyTurnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTurnObject::Setup()
{
	if (OwningPlayerIndex > -1 && OwningPlayerIndex < GManager->Players.Num())
	{
		GManager->OnTryProgressTurn[OwningPlayerIndex].AddUniqueDynamic(this, &AMyTurnObject::TurnAction);
		GManager->OnRoundStart[OwningPlayerIndex].AddUniqueDynamic(this, &AMyTurnObject::Reset);
		//GManager->Players[OwningPlayerIndex].OwningObjects.Add(this);
	}
	GManager->TurnObjects.Add(this);

	// Bind listeners in game manager
	OnFinishAction.AddDynamic(GManager, &AMyGameManager::CheckTurn);
}

void AMyTurnObject::ApplyDamage(int Damage)
{
	Health -= FMath::Abs(Damage);
	if (Health <= 0)
	{
		Health = 0;
		DestroySelf();
	}
}

void AMyTurnObject::DestroySelf()
{
	// Override this
	UE_LOG(LogTemp, Warning, TEXT("Destroy"));
	GManager->TurnObjects.Remove(this);
	Destroy();
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