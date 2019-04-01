// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgriculturalField.generated.h"

UCLASS()
class UNNAMED_API AAgriculturalField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgriculturalField();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Setup)
	bool IsOwned = false;
	TArray<AActor*> Children;


	//Static Mesh Component to interact with
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CustomMeshComponent;
	//Function to handle the interaction
	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable)
	void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);

public:	
	void SetOwned(bool value);
	bool GetOwned();
	void Buy();
};
