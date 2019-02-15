// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class UNNAMED_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int16 GetCornNumber();
	void SetCornNumber(int8 Number);
	int16 GetWheatNumber();
	void SetWheatNumber(int8 Number);

private:
	int16 CornNumber = 0;
	int16 WheatNumber = 0;


};
