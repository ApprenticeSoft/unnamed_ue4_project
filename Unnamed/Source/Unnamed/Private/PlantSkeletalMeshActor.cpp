// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantSkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyGameStateBase.h"
#include "Sol.h"


APlantSkeletalMeshActor::APlantSkeletalMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;


	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 7"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 8"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 9"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 10"), 1);

	/*
	auto map = GetSkeletalMeshComponent()->GetMorphTargetCurves();
	TArray<FName> array;
	map.GenerateKeyArray(array);
	UE_LOG(LogTemp, Warning, TEXT("Morph target map num: %i"), map.Num());

	for(int i = 0; i < array.Num(); i++)
		UE_LOG(LogTemp, Warning, TEXT("Key: %s"), *array[i].ToString());
	*/
}

// Called when the game starts or when spawned
void APlantSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Retourne le premier components de la class voulue
	auto Mesh = FindComponentByClass<USkeletalMeshComponent>();
	TArray<UMaterialInterface*> Materials = Mesh->GetMaterials();

	for (int i = 0; i < Materials.Num(); i++)
	{
		DynamicMaterials.Add(UMaterialInstanceDynamic::Create(Materials[i], NULL));
		Mesh->SetMaterial(i, DynamicMaterials[i]);
	}
	
	SetActorEnableCollision(true);
}

void APlantSkeletalMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlantState == EPlantState::Seed) 
	{
		if (MoveToPositionDelay > 0)
		{
			MoveToPositionDelay -= GetWorld()->DeltaTimeSeconds;
			if (MoveToPositionDelay <= 0)
			{
				SetActorLocation(Position);
			}
		}

		HatchDelay -= GetWorld()->DeltaTimeSeconds;
		if (HatchDelay < 0)
			PlantState = EPlantState::Growing;

	}
	else if (PlantState == EPlantState::Growing)
	{
		// Besoin en eau de la plante
		Sol->SetHumidity(-WaterNeeds * 0.006f);

		if (Sol->GetHumidity() <= 0)
			SetPlantState(EPlantState::InteruptedGrowth);

		//if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) == 0.0f)
		if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) < FinalMorphTargetRipeTriggerValue)
			PlantState = EPlantState::Ripe;
	}
	else if (PlantState == EPlantState::Ripe)
	{
		if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) == 0.0f)
			RotDelay -= GetWorld()->DeltaTimeSeconds;
		if (RotDelay < 0) 
		{
			GetSkeletalMeshComponent()->SetGenerateOverlapEvents(false);
			GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PlantState = EPlantState::Rotten;
		}
	}
	else if (PlantState == EPlantState::Rotten)
	{
		if (Rottenness < 1.0f)
		{
			Rottenness += 0.2f * GetWorld()->DeltaTimeSeconds;
			for (UMaterialInstanceDynamic* DynamicMaterial : DynamicMaterials)
				DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), Rottenness);
		}
		else if (DisapearDelay > 0)
			DisapearDelay -= GetWorld()->DeltaTimeSeconds;
		else
			PlantState = EPlantState::Disapearing;
	}
	else if (PlantState == EPlantState::InteruptedGrowth)
	{
		InteruptedGrowthRotDelay -= GetWorld()->DeltaTimeSeconds;

		if (InteruptedGrowthRotDelay < 0)
		{
			PlantState = EPlantState::InteruptedGrowthRotten;
		}
	}
	else if (PlantState == EPlantState::InteruptedGrowthRotten)
	{
		if (Rottenness < 1.0f)
		{
			Rottenness += 0.3f * GetWorld()->DeltaTimeSeconds;
			for (UMaterialInstanceDynamic* DynamicMaterial : DynamicMaterials)
				DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), Rottenness);
		}

		SetActorScale3D(GetActorScale3D() - FVector(0, 0, 0.25f * GetWorld()->DeltaTimeSeconds));
		if (GetActorScale3D().Z <= 0)
		{
			if (!Sol) { return; }
			Sol->ClearPlants();
			Destroy();
		}
	}
	else if (PlantState == EPlantState::Disapearing)
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, -0.2f));

		if (GetActorLocation().Z < 0.0f)
		{
			if (!Sol) { return; }
			Sol->ClearPlants();
			Destroy();
		}
	}
}

void APlantSkeletalMeshActor::SetPosition(FVector NewPosition)
{
	Position = NewPosition;
}

EPlantType APlantSkeletalMeshActor::GetPlantType()
{
	return PlantType;
}

EPlantState APlantSkeletalMeshActor::GetPlantState()
{
	return PlantState;
}

void APlantSkeletalMeshActor::SetPlantState(EPlantState State)
{
	PlantState = State;
}

void APlantSkeletalMeshActor::SetSol(ASol* NewSol)
{
	Sol = NewSol;
}

void APlantSkeletalMeshActor::SetInteruptedGrowthRotDelay(float value)
{
	InteruptedGrowthRotDelay = value;
}
