// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantSkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"


APlantSkeletalMeshActor::APlantSkeletalMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), 1);
}

void APlantSkeletalMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (harvested) 
	{
		if (KeyValue < 1) {
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), KeyValue);
			//UE_LOG(LogTemp, Warning, TEXT("KeyValue = %f"), KeyValue);
			KeyValue += GetWorld()->DeltaTimeSeconds;
		}
		else 
		{
			Destroy();
		}

		GetSkeletalMeshComponent()->SetGenerateOverlapEvents(false);
		GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APlantSkeletalMeshActor::Interact() 
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction avec le mais %s!!!!"), *GetName());
	harvested = true;
}