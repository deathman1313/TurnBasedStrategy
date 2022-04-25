// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBase.h"
#include "MyGameManager.h"
#include "MyBaseUnit.h"

AMyBase::AMyBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	Name = "Base";
	ObjectType = "Base";
}

void AMyBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBase::UpdateOwner(int NewOwnerIndex)
{
	// Remove previous owner
	if (OwningPlayerIndex > -1 && OwningPlayerIndex < GManager->Players.Num())
	{
		GManager->Players[OwningPlayerIndex].OwningBases.Remove(this);
		GManager->OnTryProgressTurn[OwningPlayerIndex].RemoveDynamic(this, &AMyTurnObject::TurnAction);
		GManager->OnRoundStart[OwningPlayerIndex].RemoveDynamic(this, &AMyTurnObject::Reset);
	}
	// Add new owner
	if (NewOwnerIndex > -1 && NewOwnerIndex < GManager->Players.Num())
	{
		OwningPlayerIndex = NewOwnerIndex;
		GManager->Players[OwningPlayerIndex].OwningBases.Add(this);
		GManager->OnTryProgressTurn[OwningPlayerIndex].AddUniqueDynamic(this, &AMyTurnObject::TurnAction);
		GManager->OnRoundStart[OwningPlayerIndex].AddUniqueDynamic(this, &AMyTurnObject::Reset);
		Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", GManager->Players[NewOwnerIndex].PlayerColour);
	}
	// If no new owner
	else
	{
		OwningPlayerIndex = -1;
		Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", FColor::Transparent);
	}
}

void AMyBase::DestroySelf()
{
	//Do not destroy
}

void AMyBase::TurnAction()
{
	if (!bLocked && CurrentConstruction > 0)
	{
		if (ConstructionProgress < GManager->PossibleConstrutions[CurrentConstruction].ConstructionCost)
		{
			ConstructionProgress++;
		}
		// Check if completed construction
		if (ConstructionProgress >= GManager->PossibleConstrutions[CurrentConstruction].ConstructionCost)
		{
			// Check if tile occupied
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

bool AMyBase::SetConstruction(int NewConstruction = -1)
{
	if (NewConstruction >= 0 && NewConstruction < GManager->PossibleConstrutions.Num())
	{
		CurrentConstruction = NewConstruction;
		bPerformedAction = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool AMyBase::SpawnUnit()
{
	// Spawn new unit on tile
	AMyBaseUnit* SpawnedUnit = GetWorld()->SpawnActor<AMyBaseUnit>(GManager->PossibleConstrutions[CurrentConstruction].Unit, FVector(OnTile->GetActorLocation().X, OnTile->GetActorLocation().Y, 42.f), FRotator(0.f, 180.f, 0.f));
	// Set default values
	SpawnedUnit->OwningPlayerIndex = OwningPlayerIndex;
	SpawnedUnit->Setup();
	SpawnedUnit->OnTile = OnTile;
	OnTile->OccupyingUnit = SpawnedUnit;
	GManager->Players[OwningPlayerIndex].OwningUnits.Add(SpawnedUnit);
	SpawnedUnit->CurrentMovement = 0;
	SpawnedUnit->bPerformedAction = true;
	SpawnedUnit->bLocked = true;
	return(true);
}

void AMyBase::Reset()
{
	Super::Reset();
	// Heal Base
	if (Health < MaxHealth)
	{
		Health = Health + (MaxHealth * 0.05);
		if (Health > MaxHealth)
		{
			Health = MaxHealth;
		}
	}
	if (CurrentConstruction != -1)
	{
		bPerformedAction = true;
	}
}