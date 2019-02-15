// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */


UENUM()
enum class CurrentSeason : uint8
{
	Spring,
	Summer,
	Fall,
	Winter
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


public:
	UFUNCTION(BlueprintCallable)
	int32 GetCornNumber();
	void SetCornNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	int32 GetWheatNumber();
	void SetWheatNumber(int32 Number);
	CurrentSeason GetSeason();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	CurrentSeason Season = CurrentSeason::Spring;

private:
	int32 CornNumber = 0;
	int32 WheatNumber = 0;
	float Time = 0.0f;
	float SeasonDuration = 10.0f;
};
