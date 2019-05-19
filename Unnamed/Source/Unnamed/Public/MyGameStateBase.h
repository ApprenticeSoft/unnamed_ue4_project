// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "map"
#include "PlantSkeletalMeshActor.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECurrentSeason : uint8
{
	Winter,
	Spring,
	Summer,
	Fall
};

UENUM(BlueprintType)
enum class ECurrentMonth : uint8
{
	January,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
};

class ASol;

UCLASS()
class UNNAMED_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameStateBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void TimeTracking();
	void DetermineMonth();
	void CreateCalendar();
	void CheckPlantingMonth(ECurrentMonth CurrentMonth);
	bool CheckPlantingMonth(EPlantType PlantType, ECurrentMonth CurrentMonth);
	void setSunIntensity(float &SunIntensity);


public:
	UFUNCTION(BlueprintCallable)
	int32 GetPointNumber() const;
	void SetPointNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	ECurrentSeason GetSeason() const;
	UFUNCTION(BlueprintCallable)
	ECurrentMonth GetMonth() const;
	UFUNCTION(BlueprintCallable)
	FString GetMonthString() const;
	UFUNCTION(BlueprintCallable)
	float GetSunAngle();
	UFUNCTION(BlueprintCallable)
	float GetSunLightIntensity();
	UFUNCTION(BlueprintCallable)
	float GetNightLightBlend();
	bool GetCornSeason() const;
	bool GetWheatSeason() const;
	bool GetOatSeason() const;
	bool GetPumpkinSeason() const;
	bool GetSunflowerSeason() const;
	bool GetWatermelonSeason() const;
	float GetSunIntensity() const;
	float GetPulseValue() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void CallUpdateSeasonEvent();

	UFUNCTION(BlueprintCallable)
	void BuyLand();

	// Gestion des Objectifs
	bool SetCornObjective(int32 value);
	bool SetWheatObjective(int32 value);
	bool SetPumpkinObjective(int32 value);
	bool SetObjective(int32 &Objective, int32 value);
	int32& GetCornObjectiveReference();
	int32& GetWheatObjectiveReference();
	int32& GetPumpkinObjectiveReference();
	int32 GetCornObjective() const;
	int32 GetWheatObjective() const;
	int32 GetPumpkinObjective() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentSeason Season = ECurrentSeason::Spring;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentMonth Month = ECurrentMonth::April;
	FString MonthString = "April";
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Time = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float SunLightIntensityMax = 2.5f;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool CornSeason = true;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool WheatSeason = false;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool OatSeason = false;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool PumpkinSeason = false;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool SunflowerSeason = true;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool WatermelonSeason = true;
	float PulseValue = 0;
	float PulseCounter = 0;
	void Pulse();

	int32 PointNumber = 0;
	float SeasonDuration = 60.0f;
	float MonthDuration = SeasonDuration / 3;
	float SunIntensity;
	TArray<ECurrentMonth> CornPlantingCalendar;
	TArray<ECurrentMonth> WheatPlantingCalendar;
	TArray<ECurrentMonth> OatPlantingCalendar;
	TArray<ECurrentMonth> PumpkinPlantingCalendar;
	TArray<ECurrentMonth> SunflowerPlantingCalendar;
	TArray<ECurrentMonth> WatermelonPlantingCalendar;
	std::map < EPlantType, TArray<ECurrentMonth>> CropCalendar;

	// TEST Objectif
	int32 CornObjective = 4;
	int32 WheatObjective = 0;
	int32 PumpkinObjective = 3;
	int32 CornObjectiveIndex = 0;
	int32 WheatObjectiveIndex = 0;
	int32 PumpkinObjectiveIndex = 0;


	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ASol> SoilBlueprint;
};
