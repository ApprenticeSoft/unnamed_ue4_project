// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Water.generated.h"

class UProjectileMovementComponent;
class USkeletalMeshComponent;

UCLASS()
class UNNAMED_API AWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWater();
	void LaunchWater(float Speed);
	UFUNCTION(BlueprintCallable)
	void SetDisappear(bool value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Disappear();
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;
	bool IsDisappearing = false;

};
