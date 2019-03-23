// Fill out your copyright notice in the Description page of Project Settings.

#include "Leaves.h"
#include "Classes/Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyGameStateBase.h"
#include "SingleLeave.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALeaves::ALeaves()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALeaves::BeginPlay()
{
	Super::BeginPlay();

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	// Retourne le premier components de la class voulue
	auto Mesh = FindComponentByClass<UStaticMeshComponent>();
	auto material = Mesh->GetMaterial(0);

	// Création du Dynamic Material Instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	Mesh->SetMaterial(0, DynamicMaterial);

	SetActorScale3D(FVector(0));
}

// Called every frame
void ALeaves::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Grow();
	ChangeColor();
	SpawnLeaves();

}

void ALeaves::Grow()
{
	if (GameState->GetMonth() == ECurrentMonth::April && GetActorScale3D().X < Scale)
	{
		SetActorScale3D(GetActorScale3D() + 0.05f * GetWorld()->DeltaTimeSeconds);
	}
}

void ALeaves::ChangeColor()
{
	if (GameState->GetMonth() == ECurrentMonth::September)
	{
		ColorChangeDelay -= GetWorld()->DeltaTimeSeconds;

		if(ColorChangeDelay <= 0 && ColorBlend < 1)
		{
			ColorBlend += 0.12f * GetWorld()->DeltaTimeSeconds;
			DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), ColorBlend);
		}
	}
}

void ALeaves::SpawnLeaves()
{
	if (ColorBlend >= 1 && GetActorScale3D().X > 0)
	{
		DisapearDelay -= GetWorld()->DeltaTimeSeconds;
		if(DisapearDelay < 0)
			SetActorScale3D(GetActorScale3D() - 0.05f * GetWorld()->DeltaTimeSeconds);

		SpawnLeaveDelay += GetWorld()->DeltaTimeSeconds;
		if (SpawnLeaveDelay > 2)
		{
			SpawnLeaveDelay = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.9f);

			FVector Location, Extend, RandomVector, SpawnLocation;
			GetActorBounds(false, Location, Extend);

			RandomVector = FVector(	UKismetMathLibrary::RandomFloatInRange(-0.8f, 0.8f) * Extend.X,
									UKismetMathLibrary::RandomFloatInRange(-0.8f, 0.8f) * Extend.Y,
									UKismetMathLibrary::RandomFloatInRange(-0.5f, 0.5f) * Extend.Z);
			SpawnLocation = Location + RandomVector;

			auto Leave = GetWorld()->SpawnActor<ASingleLeave>(	SingleLeaveBluePrint,
																SpawnLocation,
																UKismetMathLibrary::RandomRotator());
		}

		if (GetActorScale3D().X <= 0)
		{
			SetActorScale3D(FVector(0));
			DisapearDelay = 10;
			ColorChangeDelay = 10;
			ColorBlend = 0;
			DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), ColorBlend);
		}
	}
}

void ALeaves::SetScale(float value)
{
	Scale = value;
}

void ALeaves::SetFallColor(float value)
{
	DynamicMaterial->SetScalarParameterValue(TEXT("FallColorBlend"), value);
}

