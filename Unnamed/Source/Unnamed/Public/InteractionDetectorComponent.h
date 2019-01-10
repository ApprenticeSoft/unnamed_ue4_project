// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionDetectorComponent.generated.h"

class UCapsuleComponent;
class AActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNNAMED_API UInteractionDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionDetectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleDetector = nullptr;
	TArray<AActor*> OverlappingActors;

public:
	UPROPERTY(BlueprintReadOnly)
	FString InteractionName = "";
	void setInteractionName(FString Name);
	FString getInteractionName();
	TArray<AActor*> getOverlappingActors();
};
