// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarvestedPlant.generated.h"

class UProjectileMovementComponent;
class AMyGameStateBase;
class ABasket;
class APoint;

UENUM(BlueprintType)
enum class ECropType : uint8
{
	Corn,
	Wheat,
	Pumpkin,
	Sunflower
};

UCLASS()
class UNNAMED_API AHarvestedPlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHarvestedPlant();
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APoint> PointBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void TriggerDisappear();
	void Resize();
	bool IsDisappearing = false;
	bool IsInDemand = false;
	float DisappearDelay = 4.0f;
	float Scale = 1.0f;
	float NewScale = 1.0f;

	AMyGameStateBase* GameState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	ECropType CropType = ECropType::Corn;
	void UpdateObjective();

	// Test UProjectileMovementComponent. Peut être à enlever
	UProjectileMovementComponent* ProjectileMovement = nullptr;
	bool Thrown = false;
	void ThrownInBasket();
	ABasket* Basket = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool MoveToLocation(FVector Location, float Treshold);
	void SetThrown(bool value);
	void SetScale(float value);

	void LaunchCrop(FVector Direction, float Speed);

};
