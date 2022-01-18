// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBaseUnit.h"
#include "Components/StaticMeshComponent.h"
#include "MyTile.h"

// Sets default values
AMyBaseUnit::AMyBaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetWorldScale3D(FVector(10.f, 10.f, 10.f));

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

	if (Moving)
	{
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), MovementLocation, DeltaTime, 5);
		SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
		if (GetActorLocation().Equals(FVector(MovementLocation.X, MovementLocation.Y, GetActorLocation().Z), 5.f))
		{
			// Teleport to correct location
			UE_LOG(LogTemp, Warning, TEXT("EndMove"));
			Moving = false;
			ProcessMovement();
		}
	}
}

// Called to bind functionality to input
void AMyBaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMyBaseUnit::ProcessMovement()
{
	if (MovementQueue.Num() >= 2)
	{
		if (MovementQueue[0] == OnTile)
		{
			if (CurrentMovement > 0)
			{
				// Set location to move to
				MovementLocation = MovementQueue[1]->GetActorLocation();
				// Remove old tile position refs
				MovementQueue[0]->OccupyingUnit = nullptr;
				// And add new ones
				MovementQueue[1]->OccupyingUnit = this;
				OnTile = MovementQueue[1];
				// Start Movement
				CurrentMovement -= 1;
				MovementQueue.RemoveAt(0);
				Moving = true;
				return(true);
			}
		}
	}
	else
	{
		MovementQueue.Empty();
	}
	return(false);
}