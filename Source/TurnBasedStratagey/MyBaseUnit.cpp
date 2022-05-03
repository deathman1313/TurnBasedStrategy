// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBaseUnit.h"

#include <exception>

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyTile.h"
#include "MyBuilding.h"
#include "MyGameManager.h"
#include "MyTopDownCamera.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyBaseUnit::AMyBaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	Mesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	Mesh->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	UIInfoLocation = CreateDefaultSubobject<USceneComponent>(TEXT("UIInfoLocation"));
	UIInfoLocation->SetupAttachment(Capsule);

	Name = "Base Unit";
	ObjectType = "Unit";
}

// Called when the game starts or when spawned
void AMyBaseUnit::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyBaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move unit
	if (bMoving)
	{
		// Interp between start and end location and set location
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MovementLocation, DeltaTime, Speed);
		SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
		// If close to final destination
		if (GetActorLocation().Equals(FVector(MovementLocation.X, MovementLocation.Y, GetActorLocation().Z), 5.f))
		{
			UE_LOG(LogTemp, Warning, TEXT("EndMove"));
			// Move to final location
			SetActorLocation(FVector(MovementLocation.X, MovementLocation.Y, GetActorLocation().Z));
			bMoving = false;
			ProcessMovement(false);
		}
	}
}

// Called to bind functionality to input
void AMyBaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMyBaseUnit::ProcessMovement(bool bFirstPass)
{
	// Update location and path meshes if selected by player
	AMyTopDownCamera* HumanPlayerCharacter = Cast<AMyTopDownCamera>(GManager->Players[OwningPlayerIndex].PlayerController->GetPawn());
	if (HumanPlayerCharacter)
	{
		if (HumanPlayerCharacter->SelectedUnit == this)
		{
			GManager->CreatePath(MovementQueue);
			GManager->CreateSelector(OnTile);
		}
	}
	// Make sure path is valid
	if (MovementQueue.Num() >= 2)
	{
		if (MovementQueue[0] == OnTile)
		{
			// Check available movement
			if (CurrentMovement > 0)
			{
				// Validate path
				MovementQueue = GManager->Pathfinding->ValidatePath(MovementQueue, UnitLayer, OwningPlayerIndex);
				if (MovementQueue.Num() > 0)
				{
					// Set location to move to
					MovementLocation = MovementQueue[1]->GetActorLocation();
					// Remove old tile position refs
					MovementQueue[0]->OccupyingUnit = nullptr;
					// And add new ones
					MovementQueue[1]->OccupyingUnit = this;
					OnTile = MovementQueue[1];
					// Turn to face movement direction
					SetActorRotation(UKismetMathLibrary::GetDirectionUnitVector(MovementQueue[0]->GetActorLocation(), MovementQueue[1]->GetActorLocation()).Rotation());
					// Start Movement
					CurrentMovement -= 1;
					MovementQueue.RemoveAt(0);
					// Attempt to take base
					OnTile->TakeBase(OwningPlayerIndex);
					bMoving = true;
					return(true);
				}
			}
		}
	}
	else
	{
		MovementQueue.Empty();
	}
	// Check if movement occurred
	if (bFirstPass)
	{
		return(false);
	}
	else
	{
		bPerformedAction = true;
		return(true);
	}
}

TArray<AMyTile*> AMyBaseUnit::FindTargets()
{
	TArray<AMyTile*> Targets;
    if (GManager->Tiles.FindKey(OnTile))
    {
        const FVector TileLocation = *GManager->Tiles.FindKey(OnTile);
        for (int q = -Range; q <= Range; q++)
        {
            for (int r = UKismetMathLibrary::Max(-Range, q-Range); r <= UKismetMathLibrary::Min(Range, q+Range); r++)
            {
                const int s = r - q;
                if (GManager->Tiles.Contains(FVector(TileLocation.X + q, TileLocation.Y + r, TileLocation.Z + s)))
                {
                	AMyTile* Tile = *GManager->Tiles.Find(FVector(TileLocation.X + q, TileLocation.Y + r, TileLocation.Z + s));
                	// Check if it has unit or building
                	if (Tile->OccupyingUnit)
                	{
                		if (Tile->OccupyingUnit->OwningPlayerIndex != OwningPlayerIndex)
                		{
                			Targets.AddUnique(Tile);
                		}
                	}
                	if (Tile->Building)
                	{
                		if (Tile->Building->OwningPlayerIndex != OwningPlayerIndex && Tile->Building->Health > 0)
                		{
                			Targets.AddUnique(Tile);
                		}
                	}
                }
            }
        }
    }
	return(Targets);
}

bool AMyBaseUnit::AttackTarget(AMyTile* Target)
{
	if (ValidTargets.Contains(Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnitAttack"));
		// Attack selection
		if (Target->Building)
		{
			if (Target->Building->Health > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("UnitAttackBuilding"));
				// Damage Building
				Target->Building->ApplyDamage(AttackDamage);
				// Lock unit
				bPerformedAction = true;
				bLocked = true;
				return true;
			}
		}
		if (Target->OccupyingUnit)
		{
			UE_LOG(LogTemp, Warning, TEXT("UnitAttackUnit"));
			// Damage Unit
			Target->OccupyingUnit->ApplyDamage(AttackDamage);
			// Lock unit
			bPerformedAction = true;
			bLocked = true;
			return true;
		}
	}
	return false;
}

void AMyBaseUnit::DestroySelf()
{
	if (OnTile)
	{
		OnTile->OccupyingUnit = nullptr;
	}
	if (OwningPlayerIndex > -1 && OwningPlayerIndex < GManager->Players.Num())
	{
		GManager->Players[OwningPlayerIndex].OwningUnits.Remove(this);
	}
	Super::DestroySelf();
}

void AMyBaseUnit::TurnAction() 
{
	Super::TurnAction();
}

void AMyBaseUnit::Reset()
{
	CurrentMovement = MaxMovement;
	Super::Reset();
}