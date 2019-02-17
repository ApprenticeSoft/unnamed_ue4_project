// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */


UENUM()
enum class ECurrentSeason : uint8
{
	Winter,
	Spring,
	Summer,
	Fall
};

UENUM()
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


public:
	UFUNCTION(BlueprintCallable)
	int32 GetCornNumber();
	void SetCornNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	int32 GetWheatNumber();
	void SetWheatNumber(int32 Number);
	ECurrentSeason GetSeason();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentSeason Season = ECurrentSeason::Spring;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECurrentMonth Month = ECurrentMonth::April;

private:
	int32 CornNumber = 0;
	int32 WheatNumber = 0;
	float Time = 10.0f;
	float SeasonDuration = 10.0f;
	float MonthDuration = SeasonDuration / 3;
};
