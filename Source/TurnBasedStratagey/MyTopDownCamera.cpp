// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTopDownCamera.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameManager.h"
#include "MyPathfindingManager.h"
#include "MyTile.h"
#include "MyBaseUnit.h"
#include "MyBuilding.h"

// Sets default values
AMyTopDownCamera::AMyTopDownCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Center = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));
	SetRootComponent(Center);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(Center);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->UpdatedComponent = Center;
}

// Called when the game starts or when spawned
void AMyTopDownCamera::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GameManager)
	{
		GameManager = Cast<AMyGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyGameManager::StaticClass()));
	}
}
 
// Called every frame
void AMyTopDownCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyTopDownCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup Axis Mapping
	PlayerInputComponent->BindAxis("MoveVert", this, &AMyTopDownCamera::MoveVert);
	PlayerInputComponent->BindAxis("MoveHor", this, &AMyTopDownCamera::MoveHor);
	PlayerInputComponent->BindAxis("Zoom", this, &AMyTopDownCamera::Zoom);

	// Setup Action Mapping
	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AMyTopDownCamera::Select);

}

void AMyTopDownCamera::MoveVert(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyTopDownCamera::MoveHor(float Value)
{
	if (Value != 0)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyTopDownCamera::Zoom(float Value)
{
	if (Value != 0)
	{
		float NewArmLength = SpringArm->TargetArmLength + Value;
		if (NewArmLength > MaxArmLength)
		{
			SpringArm->TargetArmLength = MaxArmLength;
		}
		else if (NewArmLength < MinArmLength)
		{
			SpringArm->TargetArmLength = MinArmLength;
		}
		else
		{
			SpringArm->TargetArmLength = NewArmLength;
		}
	}
}

void AMyTopDownCamera::Select()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// Get Clicked Tile
		float X, Y;
		PlayerController->GetMousePosition(X, Y);
		SelectedTile = GetClickedTile(FVector2D(X, Y), PlayerController);
		SelectTile(PlayerController);
	}
}

void AMyTopDownCamera::SelectTile(APlayerController* PlayerController)
{
	// Remove visual path
	TArray<AMyTile*> TempArray;
	GameManager->CreatePath(TempArray);
	// Remove visual targeting
	if (SelectedUnit)
	{
		for (AMyTile* Tile : SelectedUnit->ValidTargets)
		{
			Tile->SelectTile(false);
		}
	}
	if (SelectedTile)
	{
		switch (SelectType)
		{
		case ESelectTypes::Select:
			// Create Selector
			GameManager->CreateSelector(SelectedTile);
			// Set the SelectedUnit
			SelectedUnit = nullptr;
			if (SelectedTile->OccupyingUnit)
			{
				if (SelectedTile->OccupyingUnit->OwningPlayerIndex > -1 && SelectedTile->OccupyingUnit->OwningPlayerIndex < GameManager->Players.Num())
				{
					if (GameManager->Players[SelectedTile->OccupyingUnit->OwningPlayerIndex].PlayerController == PlayerController)
					{
						SelectedUnit = SelectedTile->OccupyingUnit;
						// Show MovementQueue for character
						GameManager->CreatePath(SelectedUnit->MovementQueue);
					}
				}
			}
			// Set the SelectedBuilding
			SelectedBuilding = nullptr;
			if (SelectedTile->Building)
			{
				if (SelectedTile->Building->OwningPlayerIndex > -1 && SelectedTile->Building->OwningPlayerIndex < GameManager->Players.Num())
				{
					if (GameManager->Players[SelectedTile->Building->OwningPlayerIndex].PlayerController == PlayerController)
					{
						SelectedBuilding = SelectedTile->Building;
					}
				}
			}
			break;
		case ESelectTypes::Move:
			if (SelectedUnit)
			{
				// Set Movement for selected unit
				SelectedUnit->MovementQueue = GameManager->Pathfinding->FindPath(SelectedUnit->OnTile, SelectedTile, SelectedUnit->UnitLayer);
				GameManager->CreatePath(SelectedUnit->MovementQueue);
			}
			SelectType = ESelectTypes::Select;
			break;
		case ESelectTypes::UnitAttack:
			if (SelectedUnit)
			{
				// If valid attack target
				if (SelectedUnit->ValidTargets.Contains(SelectedTile))
				{
					// Attack selection
					if (SelectedTile->Building)
					{
						//SelectedTile->Building->TakeDamage();
					}
					else if (SelectedTile->OccupyingUnit)
					{
						//SelectedTile->OccupyingUnit->TakeDamage();
					}
					// Lock unit
					SelectedUnit->bPerformedAction = true;
					SelectedUnit->bLocked = true;
					// Deselect unit
					SelectedUnit = nullptr;
					// Remove selector
					GameManager->CreateSelector(nullptr);
				}
			}
			SelectType = ESelectTypes::Select;
			break;
		}
	}
	else
	{
		// No tile selected
		SelectedUnit = nullptr;
		SelectedBuilding = nullptr;
		SelectType = ESelectTypes::Select;
		// Remove Selector
		GameManager->CreateSelector(nullptr);
	}
}

AMyTile* AMyTopDownCamera::GetClickedTile(FVector2D MouseLocation, APlayerController* PlayerController)
{
	FVector WorldLocation;
	FVector WorldRotation;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, MouseLocation, WorldLocation, WorldRotation);

	FHitResult Hit;
	// Trace to find clicked tile
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, (WorldRotation * 1000) + WorldLocation, ECC_GameTraceChannel1);
	if (bSuccess)
	{
		return(Cast<AMyTile>(Hit.Actor));
	}
	else
	{
		return(nullptr);
	}
}