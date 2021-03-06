// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sol.generated.h"


UENUM(BlueprintType)
enum class ESoilState : uint8
{
	Dry,
	Wet,
	Bush
};

class APlantSkeletalMeshActor;
class UMaterialInstanceDynamic;
class AMyGameStateBase;
class ABush;

UCLASS()
class UNNAMED_API ASol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASol();
	int32 GetPlantNumber();
	void AddPlant(APlantSkeletalMeshActor* Plant);
	APlantSkeletalMeshActor* PopPlant();
	APlantSkeletalMeshActor* GetPlant();
	void ClearPlants();

	void SetHumidity(float value);
	UFUNCTION(BlueprintCallable)
	float GetHumidity() const;
	float GetEvaporationFactor() const;
	void SetEvaporationFactor(float value);
	bool IsReadyToHarvest();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void SetSelected();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void SetUnselected();
	void Highlight(bool value);
	void SetOwned(bool value);
	bool IsSoilOwned();

	ESoilState GetSoilState();
	void SetSoilState(ESoilState State);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ESoilState SoilState = ESoilState::Bush;
	AMyGameStateBase* GameState;

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	//UClass* ProjectileBluePrint;		// UClass* va afficher la totalitÚs des classes disponibles dans le blueprint alors que TSubclassOf<> n'affiche que la classe choisie
	TSubclassOf<ABush> BushBlueprint;
	ABush* Bush = nullptr;
	TArray<APlantSkeletalMeshActor*> PlanteSurLeSol;
	UMaterialInstanceDynamic* DynamicMaterial;
	float const HumidityMax = 100.0f;
	float Humidity = 100.0f;
	float EvaporationFactor = 1.3f;
	void UpdateHumidity();
	void NotifyDryness();
	float DrynessWarningIndex = 0;
	bool IsOwned = true;
};
