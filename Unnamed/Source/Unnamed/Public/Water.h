// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Water.generated.h"

class UProjectileMovementComponent;

UCLASS()
class UNNAMED_API AWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWater();
	void LaunchWater(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UProjectileMovementComponent* ProjectileMovement = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
