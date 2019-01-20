// Fill out your copyright notice in the Description page of Project Settings.

#include "Sol.h"
#include "PlantSkeletalMeshActor.h"

// Sets default values
ASol::ASol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASol::Interact()
{
	if (PlanteSurLeSol.Num() == 0) 
	{
		auto Mais = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(MaisBlueprint,
			GetActorLocation(),
			GetActorRotation());
		PlanteSurLeSol.Add(Mais);
		UE_LOG(LogTemp, Warning, TEXT("PlanteSurLeSol: %i"), PlanteSurLeSol.Num());
	}
	else 
	{
		PlanteSurLeSol[0]->Interact();
		PlanteSurLeSol.RemoveAt(0);
		UE_LOG(LogTemp, Warning, TEXT("PlanteSurLeSol: %i"), PlanteSurLeSol.Num());
	}
}

