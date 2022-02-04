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
	OwningPlayer = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AMyBaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving)
	{
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MovementLocation, DeltaTime, Speed);
		SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));
		if (GetActorLocation().Equals(FVector(MovementLocation.X, MovementLocation.Y, GetActorLocation().Z), 5.f))
		{
			UE_LOG(LogTemp, Warning, TEXT("EndMove"));
			SetActorLocation(FVector(MovementLocation.X, MovementLocation.Y, GetActorLocation().Z));
			bMoving = false;
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
	GameManager->CreatePath(MovementQueue);
	GameManager->CreateSelector(OnTile);
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
	bPerformedAction = true;
	return(false);
}

void AMyBaseUnit::TurnAction() 
{
	Super::TurnAction();
}

void AMyBaseUnit::Reset()
{
	CurrentMovement = MaxMovement;
	// This doesnt work
	Super::Reset();
}