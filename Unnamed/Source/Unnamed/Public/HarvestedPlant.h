// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestedPlant.generated.h"

class ABasket;

UCLASS()
class UNNAMED_API AHarvestedPlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestedPlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool Thrown = false;
	ABasket* Basket = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool MoveToLocation(FVector Location, float Treshold);
	void SetThrown(bool value);

};
