// Fill out your copyright notice in the Description page of Project Settings.

#include "EndTurnBTTask.h"
#include "MyGameManager.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UEndTurnBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	if (GameManager)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("CurrentTurn"), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("TurnFinished"), false);
		GameManager->NextTurn();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}