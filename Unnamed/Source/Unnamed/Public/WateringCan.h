// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WateringCan.generated.h"

class UStaticMeshComponent;
class AWater;

UCLASS()
class UNNAMED_API AWateringCan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWateringCan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* Mesh;
	float Scale = 0.1f;
	float NewScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AWater> WaterBluePrint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AdjustSize();
	void Activate();
	void Deactivate();
	void PourWater();
};
