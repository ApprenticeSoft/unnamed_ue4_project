// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "MySkeletalMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class UNNAMED_API AMySkeletalMeshActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	float KeyValue;
};
