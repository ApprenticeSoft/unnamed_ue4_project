// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Machete.generated.h"

UCLASS()
class UNNAMED_API AMachete : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMachete();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* Mesh;
	float Scale = 0.1f;
	float NewScale = 0.1f;

public:
	void AdjustSize();
	void Activate();
	void Deactivate();

};
