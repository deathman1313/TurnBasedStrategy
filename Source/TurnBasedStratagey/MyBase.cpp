// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBase.h"
#include "MyGameManager.h"
#include "MyBaseUnit.h"

AMyBase::AMyBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	Name = "Base";
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
	if (GManager->PlayerColours.Contains(NewOwner))
	{
		Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", GManager->PlayerColours[NewOwner]);
	}
}

void AMyBase::TurnAction()
{
	if (!bLocked && CurrentConstruction > 0)
	{
		if (ConstructionProgress < GManager->PossibleConstrutions[CurrentConstruction].ConstructionCost)
		{
			ConstructionProgress++;
		}
		if (ConstructionProgress >= GManager->PossibleConstrutions[CurrentConstruction].ConstructionCost)
		{
			if (!OnTile->OccupyingUnit)
			{
				ConstructionProgress -= GManager->PossibleConstrutions[CurrentConstruction].ConstructionCost;
				// Spawn constructed unit
				SpawnUnit();
				CurrentConstruction = -1;
			}
			else
			{
				// Display could not spawn message
			}
		}
	}
	Super::TurnAction();
}

bool AMyBase::SpawnUnit()
{
	AMyBaseUnit* SpawnedUnit = GetWorld()->SpawnActor<AMyBaseUnit>(GManager->PossibleConstrutions[CurrentConstruction].Unit, FVector(OnTile->GetActorLocation().X, OnTile->GetActorLocation().Y, 65.f), FRotator(0.f, 180.f, 0.f));
	SpawnedUnit->OwningPlayer = OwningPlayer;
	SpawnedUnit->OnTile = OnTile;
	OnTile->OccupyingUnit = SpawnedUnit;
	SpawnedUnit->CurrentMovement = 0;
	SpawnedUnit->bPerformedAction = true;
	SpawnedUnit->bLocked = true;
	return(true);
}

void AMyBase::Reset()
{
	Super::Reset();
	if (CurrentConstruction != -1)
	{
		bPerformedAction = true;
	}
}