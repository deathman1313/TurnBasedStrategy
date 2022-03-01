// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMountain.h"

AMyMountain::AMyMountain()
{
	Name = "Mountain";
	MaxHealth = 9999;
	Health = MaxHealth;
}

void AMyMountain::BeginPlay()
{
	Super::BeginPlay();

}

void AMyMountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyMountain::TurnAction()
{
	bPerformedAction = true;
	Super::TurnAction();
}

void AMyMountain::Reset()
{
	Super::Reset();
}