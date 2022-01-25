// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBase.h"

AMyBase::AMyBase()
{
	//PrimaryActorTick.bCanEverTick = true;

}

void AMyBase::BeginPlay()
{
	Super::BeginPlay();

	// Replace with something else
	if (!OwningPlayer)
	{
		OwningPlayer = GetWorld()->GetFirstPlayerController();
	}
}

void AMyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}