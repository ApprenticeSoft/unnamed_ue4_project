// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"


AMyGameStateBase::AMyGameStateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Game State Base BeginPlay"));
}
void AMyGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Game State Base Tick"));
}

int32 AMyGameStateBase::GetCornNumber()
{
	return CornNumber;
}

void AMyGameStateBase::SetCornNumber(int32 Number)
{
	CornNumber = Number;
}

int32 AMyGameStateBase::GetWheatNumber()
{
	return WheatNumber;
}

void AMyGameStateBase::SetWheatNumber(int32 Number)
{
	WheatNumber = Number;
}

