// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTile.generated.h"

UCLASS()
class TURNBASEDSTRATAGEY_API AMyTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		// Row index
		int I;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		// Collumn index
		int J;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		float F = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		int G = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		float H = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		AMyTile* PreviousTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		TArray<AMyTile*> Neighbours;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
