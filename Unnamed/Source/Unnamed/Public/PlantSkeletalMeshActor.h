// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "PlantSkeletalMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class UNNAMED_API APlantSkeletalMeshActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
	

public:
	APlantSkeletalMeshActor();
	void Interact();

protected:
	virtual void Tick(float DeltaTime) override;
	float KeyValue = 0.f;
	bool harvested = false;

};
