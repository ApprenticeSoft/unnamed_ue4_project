// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantSkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"


APlantSkeletalMeshActor::APlantSkeletalMeshActor()
{
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), 1);
}