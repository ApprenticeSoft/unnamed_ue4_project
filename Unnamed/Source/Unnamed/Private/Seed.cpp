// Fill out your copyright notice in the Description page of Project Settings.

#include "Seed.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASeed::ASeed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ASeed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Seed"));
}

void ASeed::LaunchSeed(float Speed)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile fires at %f"), Speed);
	auto Direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(FVector(1, 0, 0), 40);
	//ProjectileMovement->SetVelocityInLocalSpace(FVector(Direction.X, Direction.Y, 0) * Speed);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1, 0, 0) * Speed);
	ProjectileMovement->Activate();

}

