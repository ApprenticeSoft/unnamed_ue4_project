// Fill out your copyright notice in the Description page of Project Settings.

#include "Merchant.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMerchant::AMerchant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMerchant::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ActorTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pas de target!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *ActorTarget->GetFullName());
	}


}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ActorTarget)
		MoveToLocation(ActorTarget);
}

// Called to bind functionality to input
void AMerchant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


bool AMerchant::MoveToLocation(AActor * Target, float Treshold, bool ColideWithTarget)
{
	if (!Target) { return false; }
	Target->SetActorEnableCollision(ColideWithTarget);

	float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	if (Distance > Treshold)
		AddMovementInput(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Vector());

	return Distance > Treshold;
}

