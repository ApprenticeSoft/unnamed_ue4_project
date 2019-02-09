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

	if (IsHarvested)
	{
		if (KeyValue < 1) {
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), KeyValue);
			KeyValue += GetWorld()->DeltaTimeSeconds;
		}
		else
		{
			Destroy();
		}

		GetSkeletalMeshComponent()->SetGenerateOverlapEvents(false);
		GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (RotDelay < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("La plante est pourrie!"));
	}
	// Si la plante à fini sa croissance, un délai est appliqué avant qu'elle pourrisse
	else if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) == 0.0f)
	{
		RotDelay -= GetWorld()->DeltaTimeSeconds;
		UE_LOG(LogTemp, Warning, TEXT("RotDelay = %f"), RotDelay);
	}
}

bool APlantSkeletalMeshActor::IsRipe()
{
	if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) < 0.5f)
		return true;
	else
		return false;
}

void APlantSkeletalMeshActor::SetHarvested(bool Harvested)
{
	IsHarvested = Harvested;
}

void APlantSkeletalMeshActor::SetGrown(bool Grown)
{
	IsGrown = Grown;
}