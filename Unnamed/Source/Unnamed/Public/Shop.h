// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

UCLASS()
class UNNAMED_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void OpenShop();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Shop;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
