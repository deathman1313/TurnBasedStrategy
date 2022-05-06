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
		if (Unit->MovementQueue.Num() == 0)
		{
			AMyTile* MovementTile = nullptr;
			bool Loop = true;
			int Insurace = 0;
			// Find current space value
			int BestSpaceLocations = 0;
			TArray<AMyTile*> TilesInRangeOfStart = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, Unit->Range);
			for (AMyTile* CheckingTile : TilesInRangeOfStart)
			{
				if (CheckingTile->OccupyingUnit)
				{
					if (CheckingTile->OccupyingUnit->OwningPlayerIndex != Unit->OwningPlayerIndex)
					{
						BestSpaceLocations++;
					}
				}
				else if (CheckingTile->Building)
				{
					if (CheckingTile->Building->OwningPlayerIndex != Unit->OwningPlayerIndex && CheckingTile->Building->OwningPlayerIndex >= 0)
					{
						BestSpaceLocations++;
					}
				}
			}
			// Check for nearby objects
			TArray<AMyTile*> TilesInRange = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, 5);
			for (AMyTile* CheckingTile : TilesInRange)
			{
				// If valid movement location
				// Maybe change to find path?
				if (CheckingTile->bTraversable && !CheckingTile->OccupyingUnit && !GameManager->Pathfinding->IsBlockingBase(CheckingTile, Unit->OwningPlayerIndex))
				{
					int NewSpaceLocations = 0;
					// If Base can be taken
					if (CheckingTile->Building)
					{
						if (CheckingTile->Building->ObjectType == "Base")
						{
							if (CheckingTile->Building->OwningPlayerIndex != Unit->OwningPlayerIndex && CheckingTile->Building->Health <= 0)
							{
								NewSpaceLocations = NewSpaceLocations + 10;
							}
						}
					}
					// If lots of targets in range
					TArray<AMyTile*> TilesInRangeOfTile = GameManager->Pathfinding->GetTilesInRange(CheckingTile, Unit->Range);
					for (AMyTile* CheckingTile2 : TilesInRangeOfTile)
					{
						if (CheckingTile2->OccupyingUnit)
						{
							if (CheckingTile2->OccupyingUnit->OwningPlayerIndex != Unit->OwningPlayerIndex)
							{
								NewSpaceLocations++;
							}
						}
						else if (CheckingTile2->Building)
						{
							if (CheckingTile2->Building->OwningPlayerIndex != Unit->OwningPlayerIndex && CheckingTile2->Building->OwningPlayerIndex >= 0)
							{
								NewSpaceLocations++;
							}
						}
					}
					// Assign better movement tile
					if (NewSpaceLocations >= BestSpaceLocations)
					{
						if (BestSpaceLocations == 0 || FMath::RandRange(0, 9) != 0)
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
				while (Loop && Insurace < 200)
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
				// If no movement do nothing
				if (Insurace >= 200)
				{
					//Unit->DoNothing();
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
	TArray<AMyTile*> TilesInRange = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, Range);
	for (AMyTile* CheckingTile : TilesInRange)
	{
		if (CheckingTile->OccupyingUnit)
		{
			if (CheckingTile->OccupyingUnit->OwningPlayerIndex != Unit->OwningPlayerIndex)
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
	return NewSpaceLocations;
}