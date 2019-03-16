// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slate_Display.generated.h"


UENUM(BlueprintType)
enum class ETargetedPlant : uint8
{
	Corn,
	Wheat,
	Pumpkin,
	Sunflower
};

class UTextRenderComponent;
class AMyGameStateBase;

UCLASS()
class UNNAMED_API ASlate_Display : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlate_Display();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AMyGameStateBase* GameState;
	UTextRenderComponent* Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	ETargetedPlant TargetedPlant = ETargetedPlant::Corn;

	int32 Objective = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetText(int32 value);
	FString GetText() const;
	int32 GetObjective() const;
	void SetObjective();

};
