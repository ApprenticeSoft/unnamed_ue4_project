// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Billboard.generated.h"

UCLASS()
class UNNAMED_API ABillboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABillboard();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	void ReadBillboard();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void ActivateWidget();

};
