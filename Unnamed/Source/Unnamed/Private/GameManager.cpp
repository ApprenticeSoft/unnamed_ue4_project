// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int16 AGameManager::GetCornNumber()
{
	return CornNumber;
}

void AGameManager::SetCornNumber(int8 Number)
{
	CornNumber = Number;
}

int16 AGameManager::GetWheatNumber()
{
	return WheatNumber;
}

void AGameManager::SetWheatNumber(int8 Number)
{
	WheatNumber = Number;
}

