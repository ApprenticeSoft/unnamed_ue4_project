// Fill out your copyright notice in the Description page of Project Settings.

#include "Bush.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "SingleLeave.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Sol.h"

// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultRotator = GetActorRotation();

	Meshes = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Meshes.Num(): %i"), Meshes.Num());
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateBack();

	if (Soil)
	{
		if (GetActorEnableCollision() != Soil->GetActorEnableCollision())
			SetActorEnableCollision(Soil->GetActorEnableCollision());
	}
}

void ABush::ClearBush()
{
	if (Meshes.Num() > 1)
	{
		Impact();

		Meshes[0]->DestroyComponent();
		Meshes.RemoveAt(0);
	}
	else
	{
		SpawnLeaves(FVector(0,0,0), FVector(1,1,1));
		if(Soil != nullptr)
			Soil->SetHumidity(100);
		Destroy();
	}
}

void ABush::Impact()
{
	FVector ActorDirection = GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector2D ActorDirection2D(ActorDirection.X, ActorDirection.Y);
	ActorDirection2D.Normalize();

	FVector2D DifferenceVector = ActorDirection2D - FVector2D(-1, 0);
	float RotationAngle = 2 * FMath::RadiansToDegrees(FMath::Atan2(DifferenceVector.X, DifferenceVector.Y));

	FVector2D LeaningDirection(10, 0);
	LeaningDirection = LeaningDirection.GetRotated(-RotationAngle);

	AddActorLocalRotation(FRotator::MakeFromEuler(FVector(LeaningDirection.X, LeaningDirection.Y, 0)));
	NewRotator = GetActorRotation();
	IsLeaning = true;


	// Apparition des feuilles
	ActorDirection2D = FVector2D(ActorDirection.X, ActorDirection.Y);

	SpawnLeaves(	FVector(ActorDirection2D.X / UKismetMathLibrary::RandomFloatInRange(1.0, 2.0),
							ActorDirection2D.Y / UKismetMathLibrary::RandomFloatInRange(1.0, 2.0),
							50.0),
					ActorDirection);
}

void ABush::RotateBack()
{
	if (IsLeaning)
	{
		LeaningAlfa = FMath::Clamp(LeaningAlfa + 12*GetWorld()->DeltaTimeSeconds, 0.0f, 1.0f);
		SetActorRotation(FMath::Lerp(NewRotator, DefaultRotator, LeaningAlfa));

		if (LeaningAlfa == 1)
		{
			LeaningAlfa = 0;
			IsLeaning = false;
		}
	}
}

void ABush::SpawnLeaves(FVector Location, FVector ImpulseDirection)
{
	for (int i = 0; i < 6; i++)
	{
		auto Leave = GetWorld()->SpawnActor<ASingleLeave>(	SingleLeaveBluePrint,
															GetActorLocation() + Location, 
															UKismetMathLibrary::RandomRotator());
		auto Mesh = Leave->FindComponentByClass<UStaticMeshComponent>();

		Mesh->AddImpulse(	UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ImpulseDirection, 120.0) * 500,
							NAME_None,
							true);
	}
}

void ABush::SetSoil(ASol* NewSoil)
{
	Soil = NewSoil;
}

ASol* ABush::GetSoil() const
{
	return Soil;
}

bool ABush::IsInteractionEnabled()
{
	if (Soil != nullptr)
	{
		if (!Soil->GetActorEnableCollision())
			return false;
	}
	return true;
}

