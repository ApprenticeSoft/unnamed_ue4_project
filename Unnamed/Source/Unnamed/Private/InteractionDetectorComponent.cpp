// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionDetectorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"

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

	TArray<AActor*> OverlappingActors;
	CapsuleDetector->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nom de l'acteur: %s"), *Actor->GetName());
	}

	UE_LOG(LogTemp, Warning, TEXT("TAILLE DU TABLEAU: %i"), OverlappingActors.Num());
}

