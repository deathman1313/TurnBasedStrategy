// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBase.h"
#include "MyGameManager.h"

AMyBase::AMyBase()
{
	//PrimaryActorTick.bCanEverTick = true;

}

void AMyBase::BeginPlay()
{
	Super::BeginPlay();

	// Replace with something else
	if (!OwningPlayer)
	{
		UpdateOwner(GetWorld()->GetFirstPlayerController());
	}
}

void AMyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBase::UpdateOwner(AController* NewOwner)
{
	OwningPlayer = NewOwner;
	//Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", FLinearColor::Blue);
	if (GameManager->PlayerColours.Contains(NewOwner))
	{
		Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", GameManager->PlayerColours[NewOwner]);
	}
}

void AMyBase::TurnAction()
{
	OnFinishAction.Broadcast(this);
}