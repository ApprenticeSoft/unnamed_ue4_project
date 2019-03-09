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


public:
	UFUNCTION(BlueprintCallable)
	int32 GetCornNumber();
	void SetCornNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	int32 GetWheatNumber();
	void SetWheatNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	ECurrentSeason GetSeason();
	UFUNCTION(BlueprintCallable)
	ECurrentMonth GetMonth();
	UFUNCTION(BlueprintCallable)
	FString GetMonthString();
	UFUNCTION(BlueprintCallable)
	float GetSunAngle();
	UFUNCTION(BlueprintCallable)
	float GetSunIntensity();
	UFUNCTION(BlueprintCallable)
	float GetNightLightBlend();
	bool GetCornSeason() const;
	bool GetWheatSeason() const;
	bool GetPumpkinSeason() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentSeason Season = ECurrentSeason::Spring;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentMonth Month = ECurrentMonth::April;
	FString MonthString = "April";
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Time = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float SunIntensityMax = 2.5f;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool CornSeason = true;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool WheatSeason = false;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool PumpkinSeason = false;

	int32 CornNumber = 0;
	int32 WheatNumber = 0;
	float SeasonDuration = 60.0f;
	float MonthDuration = SeasonDuration / 3;
	TArray<ECurrentMonth> CornPlantingCalendar;
	TArray<ECurrentMonth> WheatPlantingCalendar;
	TArray<ECurrentMonth> PumpkinPlantingCalendar;
	std::map < EPlantType, TArray<ECurrentMonth>> CropCalendar;
};
