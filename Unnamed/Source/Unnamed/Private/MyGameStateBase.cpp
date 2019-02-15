// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"
#include "Classes/Engine/World.h"


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

	TimeTracking();
}

void AMyGameStateBase::TimeTracking()
{
	Time += GetWorld()->DeltaTimeSeconds;

	if (Time > SeasonDuration * 4)
		Time -= SeasonDuration * 4;

	else if (Time > SeasonDuration * 3) {
		if (Season != CurrentSeason::Winter)
		{
			Season = CurrentSeason::Winter;
			UE_LOG(LogTemp, Warning, TEXT("Hiver"));
		}
	}
	else if (Time > SeasonDuration * 2) {
		if (Season != CurrentSeason::Fall)
		{
			Season = CurrentSeason::Fall;
			UE_LOG(LogTemp, Warning, TEXT("Automne"));
		}
	}
	else if (Time > SeasonDuration) {
		if (Season != CurrentSeason::Summer)
		{
			Season = CurrentSeason::Summer;
			UE_LOG(LogTemp, Warning, TEXT("Ete"));
		}
	}
	else {
		if (Season != CurrentSeason::Spring)
		{
			Season = CurrentSeason::Spring;
			UE_LOG(LogTemp, Warning, TEXT("Printemps"));
		}
	}
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

CurrentSeason AMyGameStateBase::GetSeason()
{
	return Season;
}

