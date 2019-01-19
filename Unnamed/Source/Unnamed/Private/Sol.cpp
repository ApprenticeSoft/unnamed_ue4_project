// Fill out your copyright notice in the Description page of Project Settings.

#include "Sol.h"

// Sets default values
ASol::ASol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASol::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction avec le sol %s!!!!"), *GetName());
}

