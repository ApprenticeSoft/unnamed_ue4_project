// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seed.generated.h"

class UProjectileMovementComponent;

UCLASS()
class UNNAMED_API ASeed : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeed();
	void LaunchSeed(float Speed);
	UFUNCTION(BlueprintCallable)
	void SetDisappear(bool value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Disappear();
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	bool IsDisappearing = false;
	float DisappearDelay = 0.5f;
};
