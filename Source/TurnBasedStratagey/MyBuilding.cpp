// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBuilding.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyBuilding::AMyBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Center = CreateDefaultSubobject<USceneComponent>(TEXT("Center"));
	SetRootComponent(Center);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Center);
	Mesh->SetWorldScale3D(FVector(10.f, 10.f, 10.f));
}

// Called when the game starts or when spawned
void AMyBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

