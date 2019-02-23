// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Basket.generated.h"

class AHarvestedPlant;
class APlantSkeletalMeshActor;

UCLASS()
class UNNAMED_API ABasket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasket();
	void AddCrop(APlantSkeletalMeshActor* Crop);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<AHarvestedPlant*> HarvestedPlants;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AHarvestedPlant> HarvestedCornBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AHarvestedPlant> HarvestedWheatBlueprint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
