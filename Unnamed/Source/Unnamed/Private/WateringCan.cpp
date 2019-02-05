// Fill out your copyright notice in the Description page of Project Settings.

#include "WateringCan.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWateringCan::AWateringCan()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWateringCan::BeginPlay()
{
	Super::BeginPlay();

	Mesh = FindComponentByClass<class UStaticMeshComponent>();
	if (Mesh)
		Mesh->SetVisibility(false);

	SetActorScale3D(FVector(Scale));
}

// Called every frame
void AWateringCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (newScale > Scale) 
	{
		Scale += GetWorld()->DeltaTimeSeconds;
		SetActorScale3D(FVector(Scale));
	}
}

void AWateringCan::Activate()
{
	if(Mesh)
		Mesh->SetVisibility(true);

	newScale = 1;
}

