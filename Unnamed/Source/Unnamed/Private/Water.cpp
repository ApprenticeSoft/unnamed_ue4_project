// Fill out your copyright notice in the Description page of Project Settings.

#include "Water.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWater::AWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AWater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWater::LaunchWater(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile fires at %f"), Speed);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1, 0, 0) * Speed);
	ProjectileMovement->Activate();

}

