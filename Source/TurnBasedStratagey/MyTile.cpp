// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTile.h"

// Sets default values
AMyTile::AMyTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(RootComponent);
	Base->SetWorldScale3D(FVector(10.f, 10.f, 10.f));
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

