// Fill out your copyright notice in the Description page of Project Settings.


#include "MyArcher.h"

AMyArcher::AMyArcher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Name = "Archer";
	Range = 2;
}

void AMyArcher::BeginPlay()
{
	Super::BeginPlay();
}

void AMyArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}