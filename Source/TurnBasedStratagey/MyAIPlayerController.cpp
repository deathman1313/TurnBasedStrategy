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
	// Set AI Traits
	switch (FMath::RandRange(0, 2))
	{
		case 1:
			AIBehaviour.Behaviour = EAIBehaviourTypes::Defensive;
			break;
		case 2:
			AIBehaviour.Behaviour = EAIBehaviourTypes::Offensive;
			break;
		default:
			AIBehaviour.Behaviour = EAIBehaviourTypes::Default;
	}
	AIBehaviour.RangerChance = FMath::RandRange(1, 9);
	AIBehaviour.WarriorChance = 10 - AIBehaviour.RangerChance;
	// Run Behaviour Tree
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

EAIBehaviourTypes AMyAIPlayerController::GetObjectBehaviour()
{
	int RandBehav = FMath::RandRange(0, 4);
	if (RandBehav != 0)
	{
		return(AIBehaviour.Behaviour);
	}
	else
	{
		return(EAIBehaviourTypes::Offensive);
	}
}

void AMyAIPlayerController::GameEnded(TArray<AController*> WinnerControllers)
{
	GetBlackboardComponent()->SetValueAsBool(FName("CurrentTurn"), false);
}

void AMyAIPlayerController::StartTurn()
{
	OwningObjects = GameManager->Players[PlayerIndex].OwningUnits;
	OwningObjects.Append(GameManager->Players[PlayerIndex].OwningBases);
	OwningObjects.Append(GameManager->Players[PlayerIndex].OwningBuildings);
	GetBlackboardComponent()->SetValueAsBool(FName("CurrentTurn"), true);
	UE_LOG(LogTemp, Warning, TEXT("AITurn"));
}