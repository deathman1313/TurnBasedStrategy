// Fill out your copyright notice in the Description page of Project Settings.

#include "InstructObjectBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyTurnObject.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UInstructObjectBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("AIAction"));
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (GameManager && AIPlayer && ObjectIndex >= 0)
	{
		AIPlayer->OwningObjects[ObjectIndex]->DoNothing();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}