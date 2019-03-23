// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Merchant.generated.h"

class ASlate_Display;
class AMyGameStateBase;
class APoint;

UCLASS()
class UNNAMED_API AMerchant : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerchant();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	ASlate_Display* CornSlate = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	ASlate_Display* WheatSlate = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	ASlate_Display* PumpkinSlate = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	AMyGameStateBase* GameState;
	void CheckObjective();

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APoint> PointBlueprint;

	UPROPERTY(BlueprintReadWrite)
	bool IsBusy = false;
	UPROPERTY(BlueprintReadWrite)
	float AngleRotation = 0;
	UPROPERTY(BlueprintReadWrite)
	AActor* InteractionTarget = nullptr;
	UFUNCTION(BlueprintCallable)
	void PickSlate(ASlate_Display * Slate);
	UFUNCTION(BlueprintCallable)
	void PutSlateBack(ASlate_Display * Slate);
	UFUNCTION(BlueprintCallable)
	void UpdateSlateText(ASlate_Display * Slate);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void UpdateSlate();
	void SpawnPoints(int32 value, FVector Color, float scale, FVector Location);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(AActor* Target, float Treshold = 10.0f, bool ColideWithTarget = false);

};
