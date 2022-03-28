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
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CommandingObject->GetClass()->GetFName().ToString());
		if (CommandingObject->GetClass() == TSubclassOf<AMyBaseUnit>())
		{
			AMyBaseUnit* CommandingUnit = Cast<AMyBaseUnit>(CommandingObject);

		}
		else if (CommandingObject->GetClass() == TSubclassOf<AMyBase>())
		{
			AMyBase* CommandingBase = Cast<AMyBase>(CommandingObject);
			// If base has no construction
			if (CommandingBase->CurrentConstruction < 0)
			{
				CommandingBase->CurrentConstruction = 1;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AIUnknownObject"));
			// If unknown object do nothing
			CommandingObject->DoNothing();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}