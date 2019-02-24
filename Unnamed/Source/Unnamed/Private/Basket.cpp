// Fill out your copyright notice in the Description page of Project Settings.

#include "Basket.h"
#include "PlantSkeletalMeshActor.h"
#include "Classes/Engine/World.h"
#include "HarvestedPlant.h"

// Sets default values
ABasket::ABasket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasket::AddCrop(APlantSkeletalMeshActor* Crop)
{
	AHarvestedPlant* plant;

	switch (Crop->GetPlantType())
	{
	case EPlantType::Corn:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedCornBlueprint,
															FVector(0, 0, 100),
															FRotator(0, 260, 0));
		break;
	case EPlantType::Wheat:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedWheatBlueprint,
															GetActorLocation(),
															FRotator(0, 260, 0));
		break;
	default:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedCornBlueprint,
															FVector(0, 0, 100),
															FRotator(0, 260, 0));
		break;
	}

	HarvestedPlants.Add(plant);
	UE_LOG(LogTemp, Warning, TEXT("Nombre de récolte: %i"), HarvestedPlants.Num());
}


