// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sol.generated.h"

class APlantSkeletalMeshActor;

UCLASS()
class UNNAMED_API ASol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Interact();

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	//UClass* ProjectileBluePrint;		// UClass* va afficher la totalités des classes disponibles dans le blueprint alors que TSubclassOf<> n'affiche que la classe choisie
	TSubclassOf<APlantSkeletalMeshActor> MaisBlueprint;
	TArray<APlantSkeletalMeshActor*> PlanteSurLeSol;

};
