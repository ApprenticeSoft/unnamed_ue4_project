// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleLeave.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyGameStateBase.h"

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

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	// Retourne le premier components de la class voulue
	auto Mesh = FindComponentByClass<UStaticMeshComponent>();
	auto material = Mesh->GetMaterial(0);

	// Création du Dynamic Material Instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	Mesh->SetMaterial(0, DynamicMaterial);

	// Couleur aléatoire
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f));

	DisapearDelay = UKismetMathLibrary::RandomFloatInRange(3.0, 5.0f);
}

// Called every frame
void ASingleLeave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Disapear();

}

void ASingleLeave::Disapear()
{
	if (GameState->GetMonth() == ECurrentMonth::December)
	{
		DisapearDelay -= GetWorld()->DeltaTimeSeconds;
		if (DisapearDelay < 0)
		{
			SetActorScale3D(GetActorScale3D() - 0.3f * GetWorld()->DeltaTimeSeconds);

			if (GetActorScale3D().X < 0)
				Destroy();
		}
	}

}

