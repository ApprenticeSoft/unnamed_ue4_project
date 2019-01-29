// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionDetectorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UInteractionDetectorComponent::UInteractionDetectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionDetectorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractionDetectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CapsuleDetector) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Capsule de detection de collision absente!"));
		return;
	}
	else 
	{
		CapsuleDetector->GetOverlappingActors(OUT OverlappingActors);
	}

	setInteractionName("");

	for (const auto* Actor : OverlappingActors)
	{
		if (Actor == nullptr) { return; }
		setInteractionName(getInteractionName() + Actor->GetName() + "\n");
	}
}

void UInteractionDetectorComponent::setInteractionName(FString Name)
{
	InteractionName = Name;
}

FString UInteractionDetectorComponent::getInteractionName()
{
	return InteractionName;
}

TArray<AActor*> UInteractionDetectorComponent::getOverlappingActors() {
	UE_LOG(LogTemp, Warning, TEXT("Size: %i"), OverlappingActors.Num())
	return OverlappingActors;
}

