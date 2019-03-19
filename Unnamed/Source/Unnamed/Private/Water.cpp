// Fill out your copyright notice in the Description page of Project Settings.

#include "Water.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"

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
	
	SkeletalMeshComponent = FindComponentByClass<USkeletalMeshComponent>();
	SetActorRotation(FRotator(0,0,0));
}

// Called every frame
void AWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Disappear();
}

void AWater::LaunchWater(float Speed)
{
	auto DirectionVector = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation().Vector();
	ProjectileMovement->SetVelocityInLocalSpace(DirectionVector * Speed);
	ProjectileMovement->Activate();
}

void AWater::Disappear()
{
	if (IsDisappearing)
	{		
		if(SkeletalMeshComponent->GetMorphTarget(FName("Key 1")) < 1)
		{
			SkeletalMeshComponent->SetMorphTarget(FName("Key 1"), SkeletalMeshComponent->GetMorphTarget(FName("Key 1")) + 5*GetWorld()->DeltaTimeSeconds);
		}
		else
		{
			SetActorLocation(GetActorLocation() - FVector(0, 0, 0.1f));

			if (GetActorLocation().Z < 98)
				Destroy();
		}
	}
}

void AWater::SetDisappear(bool value)
{
	IsDisappearing = value;
}

