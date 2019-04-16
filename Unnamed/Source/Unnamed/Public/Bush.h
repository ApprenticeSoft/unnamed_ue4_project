// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bush.generated.h"

class UActorComponent;
class ASingleLeave;
class ASol;

UCLASS()
class UNNAMED_API ABush : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABush();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ASingleLeave> SingleLeaveBluePrint;

	TArray<UActorComponent*> Meshes;
	bool IsLeaning = false;
	FRotator DefaultRotator, NewRotator;
	float LeaningAlfa = 0;
	void Impact();
	void RotateBack();
	void SpawnLeaves(FVector Location, FVector ImpulseDirection);
	ASol* Soil;

public:	
	UFUNCTION(BlueprintCallable)
	void ClearBush();
	void SetSoil(ASol* NewSoil);
};
