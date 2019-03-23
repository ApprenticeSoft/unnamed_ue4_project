// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Point.generated.h"

class UTextRenderComponent;
class AUnnamedCharacter;

UCLASS()
class UNNAMED_API APoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UTextRenderComponent* Text = nullptr;
	int32 PointValue = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Delay)
	float MoveDelay = 1.0f;
	void AddPoints();

	//TEST
	FVector PointDestination;
	AUnnamedCharacter* Character;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int32 GetPointValue();
	void SetPointValue(int32 value);
	void SetColor(uint8 R, uint8 G, uint8 B);
	bool MoveToLocation(FVector Location, float Treshold);

};
