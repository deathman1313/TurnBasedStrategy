// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWarrior.h"

AMyWarrior::AMyWarrior()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Name = "Warrior";
	MaxHealth = 40;
	Health = 40;
	AttackDamage = 10;
}

void AMyWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void AMyWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}