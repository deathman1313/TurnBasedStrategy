// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTile.h"
#include "MyGameManager.h"
#include "MyAIPlayerController.h"
#include "MyBaseUnit.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MoveUnitBTTask.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API UMoveUnitBTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
		AMyGameManager* GameManager;

		AMyAIPlayerController* AIPlayer;

		AMyBaseUnit* Unit;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int EvaluateTile(AMyTile* CheckingTile, int Range);
};
