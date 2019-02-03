// Fill out your copyright notice in the Description page of Project Settings.

#include "Sol.h"
#include "PlantSkeletalMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"
#include "GameFramework/PlayerController.h"
#include "UnnamedCharacter.h"

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
	// Retourne le premier components de la class voulue
	auto Plane = FindComponentByClass<UStaticMeshComponent>();
	auto material = Plane->GetMaterial(0);

	// Cr�ation du Dynamic Material Instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	Plane->SetMaterial(0, DynamicMaterial);
	
}

// Called every frame
void ASol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float test = 0.5f + FMath::Cos(GetWorld()->TimeSeconds)/2;
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), test);

}

int32 ASol::GetPlantNumber()
{
	return PlanteSurLeSol.Num();
}

void ASol::AddPlant(APlantSkeletalMeshActor* Plant)
{
	PlanteSurLeSol.Add(Plant);
}

APlantSkeletalMeshActor* ASol::PopPlant()
{
	return PlanteSurLeSol.Pop();
}

APlantSkeletalMeshActor* ASol::GetPlant()
{
	return PlanteSurLeSol[0];
}

