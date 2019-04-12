// Fill out your copyright notice in the Description page of Project Settings.

#include "Machete.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMachete::AMachete()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMachete::BeginPlay()
{
	Super::BeginPlay();

	Mesh = FindComponentByClass<class UStaticMeshComponent>();
	if (Mesh)
		Mesh->SetVisibility(false);

	SetActorScale3D(FVector(Scale));
	
}

// Called every frame
void AMachete::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AdjustSize();
}

void AMachete::AdjustSize()
{
	if (NewScale > Scale)
	{
		Scale += 10 * GetWorld()->DeltaTimeSeconds;
		if (Scale > NewScale)
		{
			Scale = NewScale;
		}
		SetActorScale3D(FVector(Scale));
	}
	else if (NewScale < Scale)
	{
		Scale -= 10 * GetWorld()->DeltaTimeSeconds;
		if (Scale < NewScale)
		{
			Scale = NewScale;
			if (Mesh)
				Mesh->SetVisibility(false);
		}
		SetActorScale3D(FVector(Scale));
	}
}

void AMachete::Activate()
{
	if (Mesh)
		Mesh->SetVisibility(true);

	NewScale = 1;
}

void AMachete::Deactivate()
{
	NewScale = 0.1f;
}

