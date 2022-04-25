// Fill out your copyright notice in the Description page of Project Settings.

#include "DecideUnitConstructionBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UDecideUnitConstructionBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		AMyBase* Base = Cast<AMyBase>(AIPlayer->OwningObjects[ObjectIndex]);
		// Decide whether construction should be changed
		bool bChangeConstruction = false;
		switch (Base->CurrentConstruction)
		{
		case -1:
			bChangeConstruction = true;
			break;
		case 0:
			if (FMath::RandRange(0, 3) == 0)
			{
				bChangeConstruction = true;
			}
			break;
		default:
			if (FMath::RandRange(0, 9) == 0)
			{
				bChangeConstruction = true;
			}
		}
		// Choose new construction
		if (bChangeConstruction)
		{
			int ConstructionIndex = FMath::RandRange((GameManager->Players[Base->OwningPlayerIndex].OwningUnits.Num() < 5), 3);
			if (ConstructionIndex == 0)
			{
				Base->SetConstruction(0);
			}
			else if (ConstructionIndex > 0 && ConstructionIndex <= 2)
			{
				Base->SetConstruction(1);
			}
			else if (ConstructionIndex == 3)
			{
				Base->SetConstruction(2);
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}