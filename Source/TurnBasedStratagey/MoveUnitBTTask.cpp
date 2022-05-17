// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveUnitBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyBaseUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMoveUnitBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	const int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		Unit = Cast<AMyBaseUnit>(AIPlayer->OwningObjects[ObjectIndex]);
		if (Unit->MovementQueue.Num() == 0 || FMath::RandRange(0, 4) == 0)
		{
			AMyTile* MovementTile = nullptr;
			bool Loop = true;
			int Insurace = 0;
			// Find current space value
			int BestSpaceLocations = EvaluateTile(Unit->OnTile, Unit->Range);
			// Check for nearby objects
			TArray<AMyTile*> TilesInRange = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, 5);
			for (AMyTile* CheckingTile : TilesInRange)
			{
				// If valid movement location
				// Maybe change to find path?
				if (CheckingTile->bTraversable && !CheckingTile->OccupyingUnit && !GameManager->Pathfinding->IsBlockingBase(CheckingTile, Unit->OwningPlayerIndex))
				{
					// Determine value of new space
					int NewSpaceLocations = EvaluateTile(CheckingTile, Unit->Range);
					// Assign better movement tile
					if (NewSpaceLocations > BestSpaceLocations)
					{
						if (BestSpaceLocations <= 0 || FMath::RandRange(0, 9) != 0)
						{
							MovementTile = CheckingTile;
							BestSpaceLocations = NewSpaceLocations;
						}
					}
				}
			}
			// Determine if current space is best
			if (!MovementTile && BestSpaceLocations > 0)
			{
				//Unit->DoNothing();
			}
			else
			{
				// Find path to location
				while (Loop && Insurace < 100)
				{
					// Ensure movement location
					if (!MovementTile)
					{
						MovementTile = GameManager->Tiles[FMath::RandRange(0, GameManager->Tiles.Num() - 1)];
					}
					// Find path to location
					Unit->MovementQueue = GameManager->Pathfinding->FindPath(Unit->OnTile, MovementTile, Unit->UnitLayer, Unit->OwningPlayerIndex);
					if (Unit->MovementQueue.Num() > 0)
					{
						Loop = false;
						Unit->ProcessMovement();
					}
					else
					{
						// Failsafe
						MovementTile = nullptr;
						Insurace++;
					}
				}
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

int UMoveUnitBTTask::EvaluateTile(AMyTile* CheckingTile, int Range)
{
	int NewSpaceLocations = 0;
	// If Base can be taken, prioritise space, if own base, try to move
	if (CheckingTile->Building)
	{
		if (CheckingTile->Building->ObjectType == "Base")
		{
			if (CheckingTile->Building->OwningPlayerIndex != Unit->OwningPlayerIndex && CheckingTile->Building->Health <= 0)
			{
				NewSpaceLocations += 10;
			}
			else if (CheckingTile->Building->OwningPlayerIndex == Unit->OwningPlayerIndex)
			{
				NewSpaceLocations -= 3;
			}
		}
	}
	TArray<AMyTile*> TilesInRange = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, Range);
	for (AMyTile* CheckingTile : TilesInRange)
	{
		switch (Unit->AIBehaviour)
		{
		case EAIBehaviourTypes::Defensive:
			// Prioritise own bases
			if (CheckingTile->OccupyingUnit)
			{
				if (CheckingTile->OccupyingUnit->OwningPlayerIndex == Unit->OwningPlayerIndex)
				{
					NewSpaceLocations++;
				}
				else
				{
					NewSpaceLocations += 2;
				}
			}
			else if (CheckingTile->Building)
			{
				if (CheckingTile->Building->OwningPlayerIndex == Unit->OwningPlayerIndex)
				{
					NewSpaceLocations += 5;
				}
			}
			break;
		default:
			// Prioritise enemy units
			if (CheckingTile->OccupyingUnit)
			{
				if (CheckingTile->OccupyingUnit->OwningPlayerIndex != Unit->OwningPlayerIndex)
				{
					NewSpaceLocations += 2;
				}
				else
				{
					NewSpaceLocations++;
				}
			}
			else if (CheckingTile->Building)
			{
				if (CheckingTile->Building->OwningPlayerIndex != Unit->OwningPlayerIndex && CheckingTile->Building->OwningPlayerIndex >= 0)
				{
					NewSpaceLocations++;
				}
			}
		}
	}
	return NewSpaceLocations;
}