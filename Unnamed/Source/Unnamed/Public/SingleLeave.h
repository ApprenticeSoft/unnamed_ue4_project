// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SingleLeave.generated.h"

class UMaterialInstanceDynamic;
class AMyGameStateBase;

UCLASS()
class UNNAMED_API ASingleLeave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleLeave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UMaterialInstanceDynamic* DynamicMaterial;
	AMyGameStateBase* GameState;
	void Disapear();
	float DisapearDelay = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Setup)
	bool IsBushLeave = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
