// Fill out your copyright notice in the Description page of Project Settings.

#include "WateringCan.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Water.h"

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

	AdjustSize();
}

void AWateringCan::AdjustSize()
{
	if (NewScale > Scale)
	{
		Scale += 10 * GetWorld()->DeltaTimeSeconds;
		if (Scale > NewScale)
			Scale = NewScale;
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

void AWateringCan::Activate()
{
	if (Mesh)
		Mesh->SetVisibility(true);

	NewScale = 1;
}

void AWateringCan::Deactivate()
{
	NewScale = 0.1f;
}

void AWateringCan::PourWater()
{
	auto SocketArray = FindComponentByClass<class UStaticMeshComponent>()->GetAllSocketNames();
	for (int i = 0; i < SocketArray.Num(); i++)
	{
		auto SocketTransform = FindComponentByClass<class UStaticMeshComponent>()->GetSocketTransform(SocketArray[i]);
		auto Water = GetWorld()->SpawnActor<AWater>(WaterBluePrint,
													SocketTransform.GetLocation(),
													GetActorRotation());

		Water->LaunchWater(100);
	}
}

