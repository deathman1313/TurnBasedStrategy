// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitAttackBTTask.h"
#include "MyAIPlayerController.h"
#include "MyGameManager.h"
#include "MyBaseUnit.h"
#include "MyTile.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UUnitAttackBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	const AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	const int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		static AMyBaseUnit* Unit = Cast<AMyBaseUnit>(AIPlayer->OwningObjects[ObjectIndex]);
		TArray<AMyTile*> Targets = Unit->FindTargets();
		Unit->ValidTargets = Targets;
		if (Targets.Num() > 0)
		{
			// Find best target
			AMyTile* FinalTarget = Targets[0];
			float FinalTargetHealthRatio;
			if (FinalTarget->Building)
			{
				FinalTargetHealthRatio = FinalTarget->Building->Health / FinalTarget->Building->MaxHealth;
			}
			else
			{
				FinalTargetHealthRatio = FinalTarget->OccupyingUnit->Health / FinalTarget->OccupyingUnit->MaxHealth;
			}
			for (AMyTile* Target : Targets)
			{
				if (Target->Building)
				{
					if (Target->Building->Health / Target->Building->MaxHealth < FinalTargetHealthRatio)
					{
						FinalTarget = Target;
						FinalTargetHealthRatio = Target->Building->Health / Target->Building->MaxHealth;
					}
				}
				else
				{
					if (Target->OccupyingUnit->Health / Target->OccupyingUnit->MaxHealth < FinalTargetHealthRatio)
					{
						FinalTarget = Target;
						FinalTargetHealthRatio = Target->OccupyingUnit->Health / Target->OccupyingUnit->MaxHealth;
					}
				}
			}
			// Try attacking target
			if (!Unit->AttackTarget(FinalTarget))
			{
				Unit->DoNothing();
			}
		}
		else
		{
			Unit->DoNothing();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
