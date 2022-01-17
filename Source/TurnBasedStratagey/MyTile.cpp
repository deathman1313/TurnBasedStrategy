// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTile.h"
#include "Components/SceneComponent.h"

// Sets default values
AMyTile::AMyTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	SetRootComponent(Base);
	Base->SetWorldScale3D(FVector(10.f, 10.f, 10.f));
	TileCenter = CreateDefaultSubobject<USceneComponent>(TEXT("TileCenter"));
}

// Called when the game starts or when spawned
void AMyTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

