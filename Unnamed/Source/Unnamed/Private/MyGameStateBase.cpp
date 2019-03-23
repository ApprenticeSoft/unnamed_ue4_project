// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameStateBase.h"
#include "Classes/Engine/World.h"

#include "map"
#include "PlantSkeletalMeshActor.h"


AMyGameStateBase::AMyGameStateBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Game State Base BeginPlay"));

	CreateCalendar();
}
void AMyGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeTracking();
	Pulse();
}

void AMyGameStateBase::CreateCalendar()
{
	CornPlantingCalendar.Add(ECurrentMonth::April);
	CornPlantingCalendar.Add(ECurrentMonth::May);
	CornPlantingCalendar.Add(ECurrentMonth::June);

	WheatPlantingCalendar.Add(ECurrentMonth::September);
	WheatPlantingCalendar.Add(ECurrentMonth::October);

	PumpkinPlantingCalendar.Add(ECurrentMonth::May);
	PumpkinPlantingCalendar.Add(ECurrentMonth::June);

	CropCalendar[EPlantType::Corn] = CornPlantingCalendar;
	CropCalendar[EPlantType::Wheat] = WheatPlantingCalendar;
	CropCalendar[EPlantType::Pumpkin] = PumpkinPlantingCalendar;
}

void AMyGameStateBase::CheckPlantingMonth(ECurrentMonth CurrentMonth)
{
	CornSeason = CheckPlantingMonth(EPlantType::Corn, CurrentMonth);
	WheatSeason = CheckPlantingMonth(EPlantType::Wheat, CurrentMonth);
	PumpkinSeason = CheckPlantingMonth(EPlantType::Pumpkin, CurrentMonth);
}

bool AMyGameStateBase::CheckPlantingMonth(EPlantType PlantType, ECurrentMonth CurrentMonth)
{
	int32 index;
	return CropCalendar.find(PlantType)->second.Find(CurrentMonth, index);
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

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 10) {
		if (Month != ECurrentMonth::February)
		{
			Month = ECurrentMonth::February;
			MonthString = "February";

			CheckPlantingMonth(Month);

			// Nouvel objectif: Mais, Citrouille
			if (CornObjective == 0) 
			{
				CornObjectiveIndex += 1;
				CornObjective = 3 * CornObjectiveIndex;
			}
			if (PumpkinObjective == 0)
			{
				PumpkinObjectiveIndex += 1;
				PumpkinObjective = 2 * PumpkinObjectiveIndex + 3;
			}
		}
	}
	else if (Time > MonthDuration * 9) {
		if (Month != ECurrentMonth::January)
		{
			Month = ECurrentMonth::January;
			Season = ECurrentSeason::Winter;
			MonthString = "January";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 8) {
		if (Month != ECurrentMonth::December)
		{
			Month = ECurrentMonth::December;
			MonthString = "December";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 7) {
		if (Month != ECurrentMonth::November)
		{
			Month = ECurrentMonth::November;
			MonthString = "November";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 6) {
		if (Month != ECurrentMonth::October)
		{
			Month = ECurrentMonth::October;
			Season = ECurrentSeason::Fall;
			MonthString = "October";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 5) {
		if (Month != ECurrentMonth::September)
		{
			Month = ECurrentMonth::September;
			MonthString = "September";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 4) {
		if (Month != ECurrentMonth::August)
		{
			Month = ECurrentMonth::August;
			MonthString = "August";

			CheckPlantingMonth(Month);

			// Nouvel objectif: blé
			if (WheatObjective == 0) {
				WheatObjectiveIndex += 1;
				WheatObjective = 2 * WheatObjectiveIndex + 2;
			}
		}
	}
	else if (Time > MonthDuration * 3) {
		if (Month != ECurrentMonth::July)
		{
			Month = ECurrentMonth::July;
			Season = ECurrentSeason::Summer;
			MonthString = "July";

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration * 2) {
		if (Month != ECurrentMonth::June)
		{
			Month = ECurrentMonth::June;
			MonthString = "June";

			CheckPlantingMonth(Month);

			CheckPlantingMonth(Month);
		}
	}
	else if (Time > MonthDuration) {
		if (Month != ECurrentMonth::May)
		{
			Month = ECurrentMonth::May;
			MonthString = "May";

			CheckPlantingMonth(Month);
		}
	}
	else {
		if (Month != ECurrentMonth::April)
		{
			Month = ECurrentMonth::April;
			Season = ECurrentSeason::Spring;
			MonthString = "April";

			CheckPlantingMonth(Month);
		}
	}

	setSunIntensity(SunIntensity);
}

int32 AMyGameStateBase::GetPointNumber() const
{
	return PointNumber;
}

void AMyGameStateBase::SetPointNumber(int32 Number)
{
	PointNumber = Number;
}

ECurrentSeason AMyGameStateBase::GetSeason() const
{
	return Season;
}

ECurrentMonth AMyGameStateBase::GetMonth() const
{
	return Month;
}

FString AMyGameStateBase::GetMonthString() const
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

float AMyGameStateBase::GetSunLightIntensity()
{
	float Intensity = 0.0f;

	if (Time < SeasonDuration * 3)
		Intensity = SunLightIntensityMax * SunIntensity;
	else if (Time < SeasonDuration * 3.1f)
		Intensity = 0.5f * SunLightIntensityMax * (3.1 * SeasonDuration - Time) / (0.1f * SeasonDuration);
	else if (Time < SeasonDuration * 3.9f)
		Intensity = 0.01f;
	else
		Intensity = 0.5f * SunLightIntensityMax * (Time - 3.9f * SeasonDuration) / (0.1f * SeasonDuration);

	return Intensity;
}

float AMyGameStateBase::GetSunIntensity() const
{
	return SunIntensity;
}

void AMyGameStateBase::setSunIntensity(float & SunIntensity)
{
	SunIntensity = (1.05 + FMath::Sin(2 * PI*GetSunAngle() / 360 - PI)) / 2;
}

float AMyGameStateBase::GetNightLightBlend()
{
	float Blend = 0.0f;

	if (Time < SeasonDuration * 3)
		Blend = 0.0f;
	else if (Time < SeasonDuration * 3.2f)
		Blend = (Time - 3 * SeasonDuration) / (0.2f * SeasonDuration);
	else if (Time < SeasonDuration * 3.8f)
		Blend = 1.0f;
	else
		Blend = (4 * SeasonDuration - Time) / (0.2f * SeasonDuration);

	return Blend;
}

bool AMyGameStateBase::GetCornSeason() const
{
	return CornSeason;
}

bool AMyGameStateBase::GetWheatSeason() const
{
	return WheatSeason;
}

bool AMyGameStateBase::GetPumpkinSeason() const
{
	return PumpkinSeason;
}

bool AMyGameStateBase::SetCornObjective(int32 value)
{
	CornObjective = value;

	if (value >= 0)
		return true;
	else if (CornObjective < 0)
		CornObjective = 0;

	return false;
}

bool AMyGameStateBase::SetWheatObjective(int32 value)
{
	WheatObjective = value;

	if (value >= 0)
		return true;
	else if (WheatObjective < 0)
		WheatObjective = 0;

	return false;
}

bool AMyGameStateBase::SetPumpkinObjective(int32 value)
{
	PumpkinObjective = value;

	if (value >= 0)
		return true;
	else if (PumpkinObjective < 0)
		PumpkinObjective = 0;

	return false;
}

bool AMyGameStateBase::SetObjective(int32 &Objective, int32 value)
{
	Objective = value;

	if (value >= 0)
		return true;
	else if (Objective < 0)
		Objective = 0;

	return false;
}

int32 AMyGameStateBase::GetCornObjective() const
{
	return CornObjective;
}

int32 AMyGameStateBase::GetWheatObjective() const
{
	return WheatObjective;
}

int32 AMyGameStateBase::GetPumpkinObjective() const
{
	return PumpkinObjective;
}

int32& AMyGameStateBase::GetCornObjectiveReference()
{
	return CornObjective;
}

int32& AMyGameStateBase::GetWheatObjectiveReference()
{
	return WheatObjective;
}

int32& AMyGameStateBase::GetPumpkinObjectiveReference()
{
	return PumpkinObjective;
}

void AMyGameStateBase::Pulse()
{
	PulseCounter += 8 * GetWorld()->DeltaTimeSeconds;
	if (PulseCounter > 2 * PI)
		PulseCounter -= 2 * PI;
	PulseValue = (1 + FMath::Cos(PulseCounter)) / 2;
}

float AMyGameStateBase::GetPulseValue() const
{
	return PulseValue;
}
