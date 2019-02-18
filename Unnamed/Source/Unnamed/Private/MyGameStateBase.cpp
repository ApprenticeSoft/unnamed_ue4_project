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

	DetermineMonth();
}

void AMyGameStateBase::DetermineMonth()
{
	if (Time > SeasonDuration * 4)
		Time -= SeasonDuration * 4;

	else if (Time > MonthDuration * 11) {
		if (Month != ECurrentMonth::December)
		{
			Month = ECurrentMonth::December;
			UE_LOG(LogTemp, Warning, TEXT("Automne/Decembre"));
		}
	}
	else if (Time > MonthDuration * 10) {
		if (Month != ECurrentMonth::November)
		{
			Month = ECurrentMonth::November;
			UE_LOG(LogTemp, Warning, TEXT("Automne/Novembre"));
		}
	}
	else if (Time > MonthDuration * 9) {
		if (Month != ECurrentMonth::October)
		{
			Month = ECurrentMonth::October;
			Season = ECurrentSeason::Fall;
			UE_LOG(LogTemp, Warning, TEXT("Automne/Octobre"));
		}
	}
	else if (Time > MonthDuration * 8) {
		if (Month != ECurrentMonth::September)
		{
			Month = ECurrentMonth::September;
			UE_LOG(LogTemp, Warning, TEXT("Ete/Septembre"));
		}
	}
	else if (Time > MonthDuration * 7) {
		if (Month != ECurrentMonth::August)
		{
			Month = ECurrentMonth::August;
			UE_LOG(LogTemp, Warning, TEXT("Ete/Aout"));
		}
	}
	else if (Time > MonthDuration * 6) {
		if (Month != ECurrentMonth::July)
		{
			Month = ECurrentMonth::July;
			Season = ECurrentSeason::Summer;
			UE_LOG(LogTemp, Warning, TEXT("Ete/Juillet"));
		}
	}
	else if (Time > MonthDuration * 5) {
		if (Month != ECurrentMonth::June)
		{
			Month = ECurrentMonth::June;
			UE_LOG(LogTemp, Warning, TEXT("Printemps/Juin"));
		}
	}
	else if (Time > MonthDuration * 4) {
		if (Month != ECurrentMonth::May)
		{
			Month = ECurrentMonth::May;
			UE_LOG(LogTemp, Warning, TEXT("Printemps/Mai"));
		}
	}
	else if (Time > MonthDuration * 3) {
		if (Month != ECurrentMonth::April)
		{
			Month = ECurrentMonth::April;
			Season = ECurrentSeason::Spring;
			UE_LOG(LogTemp, Warning, TEXT("Printemps/Avril"));
		}
	}
	else if (Time > MonthDuration * 2) {
		if (Month != ECurrentMonth::March)
		{
			Month = ECurrentMonth::March;
			UE_LOG(LogTemp, Warning, TEXT("Hiver/Mars"));
		}
	}
	else if (Time > MonthDuration) {
		if (Month != ECurrentMonth::February)
		{
			Month = ECurrentMonth::February;
			UE_LOG(LogTemp, Warning, TEXT("Hiver/Fevrier"));
		}
	}
	else {
		if (Month != ECurrentMonth::January)
		{
			Month = ECurrentMonth::January;
			Season = ECurrentSeason::Winter;
			UE_LOG(LogTemp, Warning, TEXT("Hiver/Janvier"));
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

ECurrentSeason AMyGameStateBase::GetSeason()
{
	return Season;
}

