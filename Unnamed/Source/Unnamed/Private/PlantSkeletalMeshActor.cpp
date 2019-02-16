// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantSkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyGameStateBase.h"


APlantSkeletalMeshActor::APlantSkeletalMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), 1);
	GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), 1);
}

// Called when the game starts or when spawned
void APlantSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Retourne le premier components de la class voulue
	auto Mesh = FindComponentByClass<USkeletalMeshComponent>();
	auto Material0 = Mesh->GetMaterial(0);
	auto Material1 = Mesh->GetMaterial(1);
	auto Material2 = Mesh->GetMaterial(2);
	UE_LOG(LogTemp, Warning, TEXT("Material0 Name: %s"), *Material0->GetFName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Material1 Name: %s"), *Material1->GetFName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Material2 Name: %s"), *Material2->GetFName().ToString());

	// Cr�ation du Dynamic Material Instance
	DynamicMaterial0 = UMaterialInstanceDynamic::Create(Material0, NULL);
	DynamicMaterial1 = UMaterialInstanceDynamic::Create(Material1, NULL);
	DynamicMaterial2 = UMaterialInstanceDynamic::Create(Material2, NULL);

	Mesh->SetMaterial(0, DynamicMaterial0);
	Mesh->SetMaterial(1, DynamicMaterial1);
	Mesh->SetMaterial(2, DynamicMaterial2);

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
		if (GetSkeletalMeshComponent()->GetMorphTarget(FName("Key 1")) == 0.0f)
			PlantState = EPlantState::Ripe;
	}
	else if (PlantState == EPlantState::Ripe)
	{
		RotDelay -= GetWorld()->DeltaTimeSeconds;
		if (RotDelay < 0)
			PlantState = EPlantState::Rotten;
	}
	else if (PlantState == EPlantState::Rotten)
	{
		if (Rottenness < 1.0f)
		{
			Rottenness += 0.2f * GetWorld()->DeltaTimeSeconds;
		}
		DynamicMaterial0->SetScalarParameterValue(TEXT("Blend"), Rottenness);
		DynamicMaterial1->SetScalarParameterValue(TEXT("Blend"), Rottenness);
		DynamicMaterial2->SetScalarParameterValue(TEXT("Blend"), Rottenness);
	}
	else if(PlantState == EPlantState::Harvested)
	{
		if (KeyValue < 1) {
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 1"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 2"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 3"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 4"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 5"), KeyValue);
			GetSkeletalMeshComponent()->SetMorphTarget(FName("Key 6"), KeyValue);
			KeyValue += GetWorld()->DeltaTimeSeconds;
		}
		else
		{
			Destroy();
		}

		GetSkeletalMeshComponent()->SetGenerateOverlapEvents(false);
		GetSkeletalMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APlantSkeletalMeshActor::Harvest()
{
	PlantState = EPlantState::Harvested;

	auto GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());
	GameState->SetCornNumber(GameState->GetCornNumber() + 1);

	// Test pour avoir acc�s � la saison en cours � partir de la plante 
	if (GameState->GetSeason() == CurrentSeason::Summer) {
		UE_LOG(LogTemp, Warning, TEXT("C'est l'�t�!!"));
	}
}

void APlantSkeletalMeshActor::SetPosition(FVector NewPosition)
{
	Position = NewPosition;
}

EPlantState APlantSkeletalMeshActor::GetPlantState()
{
	return PlantState;
}
