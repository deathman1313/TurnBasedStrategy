// Fill out your copyright notice in the Description page of Project Settings.

#include "InstructObjectBTTask.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyTurnObject.h"
#include "MyBaseUnit.h"
#include "MyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UInstructObjectBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("AIAction"));
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		AMyTurnObject* CommandingObject = AIPlayer->OwningObjects[ObjectIndex];
		if (Cast<AMyBaseUnit>(CommandingObject))
		{
			AMyBaseUnit* CommandingUnit = Cast<AMyBaseUnit>(CommandingObject);
			if (CommandingUnit->MovementQueue.Num() == 0)
			{
				// Get random tile, for testing only should be replaced
				AMyTile* SelectedTile = nullptr;
				TArray<AMyTile*> Tiles;
				GameManager->Tiles.GenerateValueArray(Tiles);
				bool Loop = true;
				while (Loop)
				{
					SelectedTile = Tiles[FMath::RandRange(0, Tiles.Num() - 1)];
					CommandingUnit->MovementQueue = GameManager->Pathfinding->FindPath(CommandingUnit->OnTile, SelectedTile, CommandingUnit->UnitLayer, CommandingUnit->OwningPlayerIndex);
					if (CommandingUnit->MovementQueue.Num() > 0)
					{
						Loop = false;
					}
				}
			}
			CommandingUnit->ProcessMovement();
			// Maybe wait for movement to resolve? Might not be neccessary
		}
		else if (Cast<AMyBase>(CommandingObject))
		{
			AMyBase* CommandingBase = Cast<AMyBase>(CommandingObject);
			// If base has no construction
			if (CommandingBase->CurrentConstruction < 0)
			{
				if (!CommandingBase->SetConstruction(1))
				{
					// If cannot set construction do nothing
					CommandingBase->DoNothing();
				}
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