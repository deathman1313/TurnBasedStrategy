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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
		class UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		TArray<AMyTile*> Neighbours;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
