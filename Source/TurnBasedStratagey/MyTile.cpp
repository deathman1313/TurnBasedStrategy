// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTile.h"
#include "MyBase.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"

// Sets default values
AMyTile::AMyTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	SetRootComponent(Base);
	Base->SetWorldScale3D(FVector(10.f, 10.f, 10.f));
	Base->SetMobility(EComponentMobility::Stationary);

	TileCenter = CreateDefaultSubobject<USceneComponent>(TEXT("TileCenter"));
	TileCenter->SetMobility(EComponentMobility::Stationary);
}

// Called when the game starts or when spawned
void AMyTile::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultMaterials = Base->GetMaterials();

}

// Called every frame
void AMyTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTile::SelectTile(bool Select)
{
	if (Select)
	{
		for (int i = 0; i < DefaultMaterials.Num(); i++)
		{
			Base->SetMaterial(i, SelectMaterial);
		}
	}
	else
	{
		for (int i = 0; i < DefaultMaterials.Num(); i++)
		{
			Base->SetMaterial(i, DefaultMaterials[i]);
		}
	}
}

bool AMyTile::TakeBase(int RivalPlayerIndex)
{
	if (Building)
	{
		if (Building->OwningPlayerIndex != RivalPlayerIndex)
		{
			if (Building->Health <= 0)
			{
				AMyBase* OccupyingBase = Cast<AMyBase>(Building);
				if (Base)
				{
					OccupyingBase->UpdateOwner(RivalPlayerIndex);
					return(true);
				}
			}
		}
	}
	return(false);
}