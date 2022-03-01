// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBaseUnit.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyTile.h"
#include "MyGameManager.h"
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

	Name = "Base Unit";
}

// Called when the game starts or when spawned
void AMyBaseUnit::BeginPlay()
{
	Super::BeginPlay();

	// FOR TESTING ONLY SHOULD BE REMOVED
	OwningPlayerIndex = 0;
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

bool AMyBaseUnit::ProcessMovement(bool bFirstPass = true)
{
	// Update location and path meshes
	// May need to check if unit is still selected
	GManager->CreatePath(MovementQueue);
	GManager->CreateSelector(OnTile);
	// Make sure path is valid
	if (MovementQueue.Num() >= 2)
	{
		if (MovementQueue[0] == OnTile)
		{
			// Check available movement
			if (CurrentMovement > 0)
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
				bMoving = true;
				return(true);
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
		FVector TileLocation = *GManager->Tiles.FindKey(OnTile);
		for (int q = TileLocation.X - Range; q <= TileLocation.X + Range; q++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d %d %d %d"), -Range, -q - Range, Range, -q + Range);
			for (int r = TileLocation.Y + UKismetMathLibrary::Max(-Range, -q-Range); r <= TileLocation.Y + UKismetMathLibrary::Min(Range, -q+Range); r++)
			{
				int s = r - q;
				UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), q, r, s);
				if (GManager->Tiles.Contains(FVector(q, r, s)))
				{
					UE_LOG(LogTemp, Warning, TEXT("Target Valid"));
					Targets.AddUnique(*GManager->Tiles.Find(FVector(q, r, s)));
				}
			}
		}
	}
	/*
	// Define temp arrays
	TArray<AMyTile*> Targets;
	TArray<AMyTile*> CheckingTargets;
	CheckingTargets.Add(OnTile);
	TArray<AMyTile*> TempTargets;
	for (int i = 0; i < Range; i++)
	{
		for (AMyTile* Tile : CheckingTargets)
		{
			for (AMyTile* Target : Tile->Neighbours)
			{
				// Check if valid target
				if ((Target->OccupyingUnit || Target->Building) && Target != OnTile)
				{
					Targets.Add(Target);
				}
				if (!Targets.Contains(Target) && !CheckingTargets.Contains(Target))
				{
					TempTargets.AddUnique(Target);
				}
			}
		}
		CheckingTargets = TempTargets;
		TempTargets.Empty();
	}
	*/
	return(Targets);
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