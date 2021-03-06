// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyTile.h"
#include "MyTopDownCamera.generated.h"

UENUM(BlueprintType)
enum class ESelectTypes : uint8
{
	Select,
	Move,
	UnitAttack,
	BuildingAttack
};

UCLASS()
class TURNBASEDSTRATAGEY_API AMyTopDownCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyTopDownCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxArmLength = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinArmLength = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Boundary1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Boundary2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyGameManager* GameManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDraggingCam = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WorldStartDragPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ActorStartDragPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMyTile* SelectedTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyBaseUnit* SelectedUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMyBuilding* SelectedBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESelectTypes SelectType = ESelectTypes::Select;

	UPROPERTY()
		TArray<FVector> PreviousLocations;

	UFUNCTION(BlueprintCallable)
		void MoveVert(float Value);

	UFUNCTION(BlueprintCallable)
		void MoveHor(float Value);

	UFUNCTION(BlueprintCallable)
		void Zoom(float Value);

	UFUNCTION(BlueprintCallable)
		void StartDragCam();

	UFUNCTION(BlueprintCallable)
		void EndDragCam();

	UFUNCTION(BlueprintCallable)
		void Select();

	UFUNCTION(BlueprintCallable)
		void SelectTile(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
		AMyTile* GetClickedTile(FVector2D MouseLocation, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
		void GameEnded(TArray<AController*> WinnerControllers);

	UFUNCTION(BlueprintImplementableEvent)
		void DisplayEndUI(bool Winner);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
