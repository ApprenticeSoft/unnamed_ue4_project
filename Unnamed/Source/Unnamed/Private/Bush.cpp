// Fill out your copyright notice in the Description page of Project Settings.

#include "Bush.h"

// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABush::ClearBush()
{
	Destroy();
}

