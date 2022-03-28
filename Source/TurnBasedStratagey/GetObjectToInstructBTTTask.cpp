// Fill out your copyright notice in the Description page of Project Settings.

#include "GetObjectToInstructBTTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UGetObjectToInstructBTTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	if (GameManager && AIPlayer)
	{
		// Increment commanding unit
		int CurrentUnit = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
		if (CurrentUnit < 0)
		{
			CurrentUnit = 0;
		}
		else
		{
			CurrentUnit++;
		}
		// Validate commanding unit
		if (CurrentUnit >= AIPlayer->OwningObjects.Num())
		{
			CurrentUnit = -1;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("TurnFinished"), true);
			UE_LOG(LogTemp, Warning, TEXT("AIEnd"));
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("CommandingObject"), CurrentUnit);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}