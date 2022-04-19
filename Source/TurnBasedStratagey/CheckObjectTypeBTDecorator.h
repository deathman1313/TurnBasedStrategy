// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "MyTurnObject.h"
#include "CheckObjectTypeBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDSTRATAGEY_API UCheckObjectTypeBTDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AMyTurnObject> CompareObject;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
