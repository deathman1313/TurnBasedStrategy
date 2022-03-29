// Fill out your copyright notice in the Description page of Project Settings.

#include "ValidateEndTurnBTTask.h"
#include "MyGameManager.h"
#include "MyTurnObject.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UValidateEndTurnBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	if (GameManager)
	{
		if (GameManager->WaitingFor.Num() > 0)
		{
			UE_LOG(LogTemp, Error, TEXT("THIS SHOULD NOT BE APPEARING"));
			for (AMyTurnObject* Object : GameManager->WaitingFor)
			{
				Object->DoNothing();
			}
			GameManager->NextTurn();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}