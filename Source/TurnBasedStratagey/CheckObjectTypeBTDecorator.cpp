// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckObjectTypeBTDecorator.h"
#include "MyAIPlayerController.h"
#include "MyGameManager.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UCheckObjectTypeBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMyGameManager* GameManager = Cast<AMyGameManager>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("GameManager")));
	AMyAIPlayerController* AIPlayer = Cast<AMyAIPlayerController>(OwnerComp.GetOwner());
	int ObjectIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt(FName("CommandingObject"));
	if (AIPlayer && GameManager && ObjectIndex >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Check Type"));
		AMyTurnObject* CommandingObject = AIPlayer->OwningObjects[ObjectIndex];
		return CommandingObject->IsA(CompareObject->StaticClass());
	}
	return false;
}