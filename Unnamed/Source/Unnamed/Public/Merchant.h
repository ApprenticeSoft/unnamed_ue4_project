// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Merchant.generated.h"

UCLASS()
class UNNAMED_API AMerchant : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerchant();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	AActor* ActorTarget = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(AActor* Target, float Treshold = 10.0f, bool ColideWithTarget = false);

};
