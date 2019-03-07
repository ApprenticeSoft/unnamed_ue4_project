// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SingleLeave.generated.h"

class UMaterialInstanceDynamic;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
