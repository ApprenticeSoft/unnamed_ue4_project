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
		if (Month != ECurrentMonth::March)
		{
			Month = ECurrentMonth::March;
			MonthString = "March";
		}
	}
	else if (Time > MonthDuration * 10) {
		if (Month != ECurrentMonth::February)
		{
			Month = ECurrentMonth::February;
			MonthString = "February";
		}
	}
	else if (Time > MonthDuration * 9) {
		if (Month != ECurrentMonth::January)
		{
			Month = ECurrentMonth::January;
			Season = ECurrentSeason::Winter;
			MonthString = "January";
		}
	}
	else if (Time > MonthDuration * 8) {
		if (Month != ECurrentMonth::December)
		{
			Month = ECurrentMonth::December;
			MonthString = "December";
		}
	}
	else if (Time > MonthDuration * 7) {
		if (Month != ECurrentMonth::November)
		{
			Month = ECurrentMonth::November;
			MonthString = "November";
		}
	}
	else if (Time > MonthDuration * 6) {
		if (Month != ECurrentMonth::October)
		{
			Month = ECurrentMonth::October;
			Season = ECurrentSeason::Fall;
			MonthString = "October";
		}
	}
	else if (Time > MonthDuration * 5) {
		if (Month != ECurrentMonth::September)
		{
			Month = ECurrentMonth::September;
			MonthString = "September";
		}
	}
	else if (Time > MonthDuration * 4) {
		if (Month != ECurrentMonth::August)
		{
			Month = ECurrentMonth::August;
			MonthString = "August";
		}
	}
	else if (Time > MonthDuration * 3) {
		if (Month != ECurrentMonth::July)
		{
			Month = ECurrentMonth::July;
			Season = ECurrentSeason::Summer;
			MonthString = "July";
		}
	}
	else if (Time > MonthDuration * 2) {
		if (Month != ECurrentMonth::June)
		{
			Month = ECurrentMonth::June;
			MonthString = "June";
		}
	}
	else if (Time > MonthDuration) {
		if (Month != ECurrentMonth::May)
		{
			Month = ECurrentMonth::May;
			MonthString = "May";
		}
	}
	else {
		if (Month != ECurrentMonth::April)
		{
			Month = ECurrentMonth::April;
			Season = ECurrentSeason::Spring;
			MonthString = "April";
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

FString AMyGameStateBase::GetMonth()
{
	return MonthString;
}

float AMyGameStateBase::GetSunAngle()
{
	if (Time < SeasonDuration * 3)
		return 180 + (Time * 180) / (SeasonDuration * 3);
	else
		return 360 + ((Time - 3*SeasonDuration) * 180) / SeasonDuration;
}

float AMyGameStateBase::GetSunIntensity()
{
	float Intensity = SunIntensityMax * (1 + FMath::Sin(2*PI*GetSunAngle()/360 - PI))/2;

	return Intensity;
}

