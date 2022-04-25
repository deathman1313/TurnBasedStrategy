// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveUnitBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyBaseUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMoveUnitBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		AMyBaseUnit* Unit = Cast<AMyBaseUnit>(AIPlayer->OwningObjects[ObjectIndex]);
		if (Unit->MovementQueue.Num() == 0)
		{
			AMyTile* MovementTile = nullptr;
			bool Loop = true;
			int Insurace = 0;
			while (Loop || Insurace < 100)
			{
				// Get Tile
				Unit->MovementQueue = GameManager->Pathfinding->FindPath(Unit->OnTile, MovementTile, Unit->UnitLayer, Unit->OwningPlayerIndex);
				if (Unit->MovementQueue.Num() > 0)
				{
					Loop = false;
				}
				Insurace++;
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}