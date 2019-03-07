// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleLeave.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASingleLeave::ASingleLeave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingleLeave::BeginPlay()
{
	Super::BeginPlay();

	/*
	SetActorEnableCollision(true);
	FindComponentByClass<class UStaticMeshComponent>()->SetSimulatePhysics(true);
	FindComponentByClass<class UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	*/

	// Retourne le premier components de la class voulue
	auto Mesh = FindComponentByClass<UStaticMeshComponent>();
	auto material = Mesh->GetMaterial(0);

	// Création du Dynamic Material Instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	Mesh->SetMaterial(0, DynamicMaterial);

	// Couleur aléatoire
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f));

	// Angle aléatoire
	SetActorRotation(FRotator(	UKismetMathLibrary::RandomIntegerInRange(0,360),
								UKismetMathLibrary::RandomIntegerInRange(0, 360), 
								UKismetMathLibrary::RandomIntegerInRange(0, 360)));

}

// Called every frame
void ASingleLeave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

