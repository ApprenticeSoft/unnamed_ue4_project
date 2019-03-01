// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestedPlant.generated.h"

class UProjectileMovementComponent;
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
	UFUNCTION(BlueprintCallable)
	void TriggerDisappear();
	void Disappear();
	bool IsDisappearing = false;
	float DisappearDelay = 4.0f;

	// Test UProjectileMovementComponent. Peut être à enlever
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	bool Thrown = false;
	void ThrownInBasket();
	ABasket* Basket = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool MoveToLocation(FVector Location, float Treshold);
	void SetThrown(bool value);

	void LaunchCrop(FVector Direction, float Speed);

};
