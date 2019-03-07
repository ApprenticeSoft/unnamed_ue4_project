// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

class ALeaves;
class AMyGameStateBase;

UCLASS()
class UNNAMED_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateLeaves();

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ALeaves> LeavesBluePrint;
	AMyGameStateBase* GameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
