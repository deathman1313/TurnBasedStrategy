// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIPlayerController.h"
#include "MyGameManager.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIPlayerController::AMyAIPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyAIPlayerController::BeginPlay()
{
	GameManager->OnGameEnd.AddUniqueDynamic(this, &AMyAIPlayerController::GameEnded);
}

void AMyAIPlayerController::Tick(float DeltaTime)
{

}

void AMyAIPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunBehaviorTree(BehaviorTree);
}

void AMyAIPlayerController::GameEnded(TArray<AController*> WinnerControllers)
{

}

void AMyAIPlayerController::StartTurn()
{
	GetBlackboardComponent()->SetValueAsBool(FName("CurrentTurn"), true);
}