// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PlantSkeletalMeshActor.generated.h"

class UMaterialInstanceDynamic;
class AHarvestedPlant;

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlantState : uint8
{
	Seed,
	Growing,
	InteruptedGrowth,
	InteruptedGrowthRotten,
	Ripe,
	Rotten,
	Disapearing
};

UENUM(BlueprintType)
enum class EPlantType : uint8
{
	Corn,
	Wheat,
	Pumpkin,
	Sunflower
};

class ASol;

UCLASS()
class UNNAMED_API APlantSkeletalMeshActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
	

public:
	APlantSkeletalMeshActor();
	UFUNCTION(BlueprintCallable)
	void SetPosition(FVector NewPosition);
	UFUNCTION(BlueprintCallable)
	EPlantState GetPlantState();
	void SetPlantState(EPlantState State);
	UFUNCTION(BlueprintCallable)
	EPlantType GetPlantType();
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AHarvestedPlant> CropBlueprint;
	void SetSol(ASol* NewSol);
	void SetInteruptedGrowthRotDelay(float value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ASol* Sol = nullptr;
	float KeyValue = 0.f;
	float Rottenness = 0;
	FVector Position = FVector(0,0,0);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float MoveToPositionDelay = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float HatchDelay = 2.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float RotDelay = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float InteruptedGrowthRotDelay = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float DisapearDelay = 10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Setup)
	float WaterNeeds = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EPlantState PlantState = EPlantState::Seed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EPlantType PlantType = EPlantType::Corn;

	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
};
