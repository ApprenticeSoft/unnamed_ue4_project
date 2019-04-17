// Fill out your copyright notice in the Description page of Project Settings.

#include "Sol.h"
#include "PlantSkeletalMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Color.h"
#include "GameFramework/PlayerController.h"
#include "UnnamedCharacter.h"
#include "MyGameStateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bush.h"

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

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	// Retourne le premier components de la class voulue
	auto Plane = FindComponentByClass<UStaticMeshComponent>();
	auto material = Plane->GetMaterial(0);

	// Création du Dynamic Material Instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
	Plane->SetMaterial(0, DynamicMaterial);
	
	ABush* Bush = GetWorld()->SpawnActor<ABush>(BushBlueprint,
												GetActorLocation(),
												FRotator(0, (float)(UKismetMathLibrary::RandomIntegerInRange(10,180)), 0));
	Bush->SetSoil(this);
}

// Called every frame
void ASol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(SoilState == ESoilState::Wet)
		UpdateHumidity();
	else if (SoilState == ESoilState::Dry)
		NotifyDryness();
}

/*
* Fonction qui retourne le nombre de plantes présentent sur le sol.
*/
int32 ASol::GetPlantNumber()
{
	return PlanteSurLeSol.Num();
}

/*
* Fonction qui permet d'ajouter une plante sur le sol.
*/
void ASol::AddPlant(APlantSkeletalMeshActor* Plant)
{
	if (!Plant) { return; }
	PlanteSurLeSol.Add(Plant);
}

/*
* Fonction qui enlève et retourne une plante du sol.
*/
APlantSkeletalMeshActor* ASol::PopPlant()
{
	return PlanteSurLeSol.Pop();
}

/*
* Fonction qui retourne la première plante du sol.
*/
APlantSkeletalMeshActor* ASol::GetPlant()
{
	return PlanteSurLeSol[0];
}

/*
* Fonction qui éfface toutes les plantes présentent sur le sol.
*/
void ASol::ClearPlants()
{
	PlanteSurLeSol.Empty();
}

/*
* Fonction qui met à jour le niveau d'humidité en fonction
* du temps, de l'intensité du soleil, et 
* d'un facteur d'évaporation qui dépend de la plante présente sur le sol.
*/
void ASol::UpdateHumidity()
{
	Humidity -= EvaporationFactor * GameState->GetSunIntensity() * GetWorld()->DeltaTimeSeconds;
	if (Humidity < 0)
	{
		Humidity = 0;
		SoilState = ESoilState::Dry;
	}
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Humidity"), Humidity / HumidityMax);
}

/*
* Fonction qui permet de signaler visuellement la sécheresse du sol.
*/
void ASol::NotifyDryness()
{
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), GameState->GetPulseValue());
}

/*
* Fonction qui permet d'ajouter une valeur donnée au niveau d'humidité
*/
void ASol::SetHumidity(float value)
{
	Humidity += value;
	if (Humidity > 100)
		Humidity = 100;

	SoilState = ESoilState::Wet;
	if (PlanteSurLeSol.Num() > 0)
		if (GetPlant()->GetPlantState() == EPlantState::InteruptedGrowth)
		{
			GetPlant()->SetPlantState(EPlantState::Growing);
			GetPlant()->SetInteruptedGrowthRotDelay(20);
		}

	// On annule le warning liée à la secheresse du sol
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 0);
}

/*
* Fonction qui retourne le niveau d'humidité du sol.
*/
float ASol::GetHumidity() const
{
	return Humidity;
}

/*
* Fonction qui retourne le facteur d'évaporation du sol.
*/
float ASol::GetEvaporationFactor() const
{
	return EvaporationFactor;
}

/*
* Fonction qui permet de modifier le facteur d'évaporation.
*/
void ASol::SetEvaporationFactor(float value)
{
	EvaporationFactor = value;
}

/*
* Fonction qui retourne "true" si la première plante sur le sol est mûre.
*/
bool ASol::IsReadyToHarvest()
{
	if (PlanteSurLeSol.Num() > 0)
		if (PlanteSurLeSol[0]->GetPlantState() == EPlantState::Ripe)
			return true;

	return  false;
}

/*
* Fonction qui retourne l'état du sol (Wet, Dry, Bush).
*/
ESoilState ASol::GetSoilState()
{
	return SoilState;
}

/*
* Fonction qui permet de modifier l'état du sol.
*/
void ASol::SetSoilState(ESoilState State)
{
	SoilState = State;
}

/*
* Fonction qui permet mettre en lumière le sol, par l'intermédiaire
* du paramètre scalaire Blend_Dry_Warning.
*/
void ASol::Highlight(bool value)
{
	if(value)
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 1);
	else
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 0);
}

