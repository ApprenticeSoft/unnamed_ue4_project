// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PlantSkeletalMeshActor.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class UNNAMED_API APlantSkeletalMeshActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
	

public:
	APlantSkeletalMeshActor();
	bool IsRipe();
	UFUNCTION(BlueprintCallable)
		void SetHarvested(bool Harvested);
	UFUNCTION(BlueprintCallable)
		void SetGrown(bool Grown);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RotDelay = 100;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	float KeyValue = 0.f;
	float Rottenness = 0;
	bool IsHarvested = false;
	bool IsGrown = false;

	UMaterialInstanceDynamic* DynamicMaterial0;
	UMaterialInstanceDynamic* DynamicMaterial1;
	UMaterialInstanceDynamic* DynamicMaterial2;

};
