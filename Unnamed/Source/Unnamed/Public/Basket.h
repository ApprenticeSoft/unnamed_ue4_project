// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Basket.generated.h"

class AHarvestedPlant;
class APlantSkeletalMeshActor;
class USkeletalMeshComponent;

UCLASS()
class UNNAMED_API ABasket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<AHarvestedPlant*> HarvestedPlants;
	TArray<FName> SocketArray;
	int SocketIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AHarvestedPlant> HarvestedCornBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AHarvestedPlant> HarvestedWheatBlueprint;


	//Test
	float TestDelay = 2.0;
	FTransform BackRelativeTransform;
	FTransform HandRelativeTransform;
	USkeletalMeshComponent* PlayerSkeletalMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttachToBack();
	void AttachToHand();
	void AddCrop(APlantSkeletalMeshActor* Crop);
	void AddCrop(AHarvestedPlant* Crop);
	void ReleaseCrop();
};
