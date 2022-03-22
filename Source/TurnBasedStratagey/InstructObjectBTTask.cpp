// Fill out your copyright notice in the Description page of Project Settings.

#include "InstructObjectBTTask.h"
#include "MyAIPlayerController.h"
#include "MyTurnObject.h"
#include "MyBaseUnit.h"
#include "MyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UInstructObjectBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("AIAction"));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && ObjectIndex >= 0)
	{
		AMyTurnObject* CommandingObject = AIPlayer->OwningObjects[ObjectIndex];
		if (Cast<AMyBaseUnit>(CommandingObject))
		{
			AMyBaseUnit* CommandingUnit = Cast<AMyBaseUnit>(CommandingObject);

		}
		else if (Cast<AMyBase>(CommandingObject))
		{
			AMyBase* CommandingBase = Cast<AMyBase>(CommandingObject);

		}
		CommandingObject->DoNothing();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;


	if (CommandingObject->GetClass() == TSubclassOf<AMyBaseUnit>())
}