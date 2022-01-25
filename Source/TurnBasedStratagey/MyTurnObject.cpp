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
	
	AMyGameManager* Test = Cast<AMyGameManager>(UGameplayStatics::GetActorOfClass(this, AMyGameManager::StaticClass()));
	Test->OnRoundStart.AddDynamic(this, &AMyTurnObject::TurnAction);
	Test->TurnObjects.Add(this);

	// Bind listener in game manager to OnFinishAction
	OnFinishAction.AddDynamic(Test, &AMyGameManager::CheckTurn);
}

// Called every frame
void AMyTurnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTurnObject::TurnAction()
{
	// Finished TurnAction
	OnFinishAction.Broadcast(this);
}