// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */

UCLASS()
class UNNAMED_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameStateBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION(BlueprintCallable)
	int32 GetCornNumber();
	void SetCornNumber(int32 Number);
	UFUNCTION(BlueprintCallable)
	int32 GetWheatNumber();
	void SetWheatNumber(int32 Number);

private:
	int32 CornNumber = 0;
	int32 WheatNumber = 0;
};
