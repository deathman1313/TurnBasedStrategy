// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBaseUnit.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyBaseUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBaseUnit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		class AMyTile* OnTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int MaxMovement = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		int CurrentMovement = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		TArray<class AMyTile*> MovementQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool Moving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		FVector MovementLocation;

	UFUNCTION(BlueprintCallable)
		bool ProcessMovement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
