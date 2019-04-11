// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Billboard.generated.h"

class UTextRenderComponent;

UCLASS()
class UNNAMED_API ABillboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABillboard();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UTextRenderComponent* TextPrice = nullptr;
	int32 Price = 0;
	float DisapearDelay = 2.0f;
	bool IsBought = false;

	void Disapear();

public:	
	void ReadBillboard();
	int32 GetPrice() const;
	void SetPrice(int32 value);
	void BuyField();

};
