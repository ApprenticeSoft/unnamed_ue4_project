// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Leaves.generated.h"

class AMyGameStateBase;
class ASingleLeave;
class UMaterialInstanceDynamic;

UCLASS()
class UNNAMED_API ALeaves : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeaves();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ASingleLeave> SingleLeaveBluePrint;
	UMaterialInstanceDynamic* DynamicMaterial;
	AMyGameStateBase* GameState;
	float ColorBlend = 0.0f;
	float ColorChangeDelay = 9.0f;
	float DisapearDelay = 10.0f;
	void Grow();
	void ChangeColor();
	void SpawnLeaves();
	float SpawnLeaveDelay = 0;
	float Scale = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetScale(float value);

};
