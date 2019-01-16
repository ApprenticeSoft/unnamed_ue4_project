// Fill out your copyright notice in the Description page of Project Settings.

#include "MySkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"


/*
AMySkeletalMeshActor::AMySkeletalMeshActor() 
{

}
*/

// Called when the game starts
void AMySkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Test skeletal mesh!"));
	//SetMorphTarget

	PrimaryActorTick.bCanEverTick = true;
	KeyValue = 0;

}

void AMySkeletalMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("test"));

	if (KeyValue < 1) {
		GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), KeyValue);
		KeyValue += GetWorld()->DeltaTimeSeconds;
	}
}