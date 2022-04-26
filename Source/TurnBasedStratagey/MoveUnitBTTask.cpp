// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveUnitBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyBaseUnit.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMoveUnitBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	const AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	const int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		AMyBaseUnit* Unit = Cast<AMyBaseUnit>(AIPlayer->OwningObjects[ObjectIndex]);
		if (Unit->MovementQueue.Num() == 0)
		{
			AMyTile* MovementTile = nullptr;
			bool Loop = true;
			int Insurace = 0;
			// Check for nearby objects
			TArray<AMyTile*> TilesInRange = GameManager->Pathfinding->GetTilesInRange(Unit->OnTile, 5);
			int BestSpaceLocations = 0;
			for (AMyTile* CheckingTile : TilesInRange)
			{
				// If valid movement location
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
			while (Loop && Insurace < 200)
			{
				// Ensure movement location
				if (!MovementTile)
				{
					TArray<AMyTile*> Tiles;
					GameManager->Tiles.GenerateValueArray(Tiles);
					MovementTile = Tiles[FMath::RandRange(0, Tiles.Num() - 1)];
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
					MovementTile = nullptr;
					Insurace++;
				}
			}
			// If no movement do nothing
			if (Insurace >= 200)
			{
				Unit->DoNothing();
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}