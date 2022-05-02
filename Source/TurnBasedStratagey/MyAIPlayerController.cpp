// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIPlayerController.h"
#include "MyGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIPlayerController::AMyAIPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyAIPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Get active GameManager
	GameManager = Cast<AMyGameManager>(UGameplayStatics::GetActorOfClass(this, AMyGameManager::StaticClass()));
	GameManager->OnGameEnd.AddUniqueDynamic(this, &AMyAIPlayerController::GameEnded);
	BehaviorTree = GameManager->AIBehaviorTree;
	RunBehaviorTree(BehaviorTree);
	GetBlackboardComponent()->SetValueAsInt(FName("PlayerIndex"), PlayerIndex);
	GetBlackboardComponent()->SetValueAsInt(FName("CommandingObject"), -1);
	GetBlackboardComponent()->SetValueAsObject(FName("GameManager"), GameManager);
}

void AMyAIPlayerController::Tick(float DeltaTime)
{

}

// Probably unnecessary
void AMyAIPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMyAIPlayerController::GameEnded(TArray<AController*> WinnerControllers)
{

}

void AMyAIPlayerController::StartTurn()
{
	OwningObjects = GameManager->Players[PlayerIndex].OwningUnits;
	OwningObjects.Append(GameManager->Players[PlayerIndex].OwningBases);
	OwningObjects.Append(GameManager->Players[PlayerIndex].OwningBuildings);
	GetBlackboardComponent()->SetValueAsBool(FName("CurrentTurn"), true);
	UE_LOG(LogTemp, Warning, TEXT("AITurn"));
	//GameManager->NextTurn();
}