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

	// Cr�ation du Dynamic Material Instance
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
* Fonction qui retourne le nombre de plantes pr�sentent sur le sol.
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
* Fonction qui enl�ve et retourne une plante du sol.
*/
APlantSkeletalMeshActor* ASol::PopPlant()
{
	return PlanteSurLeSol.Pop();
}

/*
* Fonction qui retourne la premi�re plante du sol.
*/
APlantSkeletalMeshActor* ASol::GetPlant()
{
	return PlanteSurLeSol[0];
}

/*
* Fonction qui �fface toutes les plantes pr�sentent sur le sol.
*/
void ASol::ClearPlants()
{
	PlanteSurLeSol.Empty();
}

/*
* Fonction qui met � jour le niveau d'humidit� en fonction
* du temps, de l'intensit� du soleil, et 
* d'un facteur d'�vaporation qui d�pend de la plante pr�sente sur le sol.
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
* Fonction qui permet de signaler visuellement la s�cheresse du sol.
*/
void ASol::NotifyDryness()
{
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), GameState->GetPulseValue());
}

/*
* Fonction qui permet d'ajouter une valeur donn�e au niveau d'humidit�
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

	// On annule le warning li�e � la secheresse du sol
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 0);
}

/*
* Fonction qui retourne le niveau d'humidit� du sol.
*/
float ASol::GetHumidity() const
{
	return Humidity;
}

/*
* Fonction qui retourne le facteur d'�vaporation du sol.
*/
float ASol::GetEvaporationFactor() const
{
	return EvaporationFactor;
}

/*
* Fonction qui permet de modifier le facteur d'�vaporation.
*/
void ASol::SetEvaporationFactor(float value)
{
	EvaporationFactor = value;
}

/*
* Fonction qui retourne "true" si la premi�re plante sur le sol est m�re.
*/
bool ASol::IsReadyToHarvest()
{
	if (PlanteSurLeSol.Num() > 0)
		if (PlanteSurLeSol[0]->GetPlantState() == EPlantState::Ripe)
			return true;

	return  false;
}

/*
* Fonction qui retourne l'�tat du sol (Wet, Dry, Bush).
*/
ESoilState ASol::GetSoilState()
{
	return SoilState;
}

/*
* Fonction qui permet de modifier l'�tat du sol.
*/
void ASol::SetSoilState(ESoilState State)
{
	SoilState = State;
}

/*
* Fonction qui permet mettre en lumi�re le sol, par l'interm�diaire
* du param�tre scalaire Blend_Dry_Warning.
*/
void ASol::Highlight(bool value)
{
	if(value)
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 1);
	else
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend_Dry_Warning"), 0);
}

