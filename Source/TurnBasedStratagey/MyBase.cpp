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

}

void AMyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBase::UpdateOwner(int NewOwnerIndex)
{
	//Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", FLinearColor::Blue);
	if (NewOwnerIndex > -1 && NewOwnerIndex < GManager->Players.Num())
	{
		OwningPlayerIndex = NewOwnerIndex;
		for (FPlayerInfo Player : GManager->Players)
		{
			if (Player.PlayerController == GManager->Players[NewOwnerIndex].PlayerController)
			{
				Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", Player.PlayerColour);
			}
		}
	}
	else
	{
		OwningPlayerIndex = -1;
		Mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Colour", FColor::Transparent);
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

bool AMyBase::SpawnUnit()
{
	// Spawn new unit on tile
	AMyBaseUnit* SpawnedUnit = GetWorld()->SpawnActor<AMyBaseUnit>(GManager->PossibleConstrutions[CurrentConstruction].Unit, FVector(OnTile->GetActorLocation().X, OnTile->GetActorLocation().Y, 62.f), FRotator(0.f, 180.f, 0.f));
	// Set default values
	SpawnedUnit->OwningPlayerIndex = OwningPlayerIndex;
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