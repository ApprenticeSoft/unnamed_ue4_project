// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestedPlant.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "UnnamedCharacter.h"
#include "Basket.h"

// Sets default values
AHarvestedPlant::AHarvestedPlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarvestedPlant::BeginPlay()
{
	Super::BeginPlay();

	Basket = dynamic_cast<AUnnamedCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetBasket();

	if (!Basket)
		UE_LOG(LogTemp, Warning, TEXT("Pas de Basket!!!"));


}

// Called every frame
void AHarvestedPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Thrown) 
	{
		Thrown = MoveToLocation(Basket->GetActorLocation(), 10);
		if (!Thrown)
		{
			Basket->AddCrop(this);
		}
	}	

}

bool AHarvestedPlant::MoveToLocation(FVector Location, float Treshold)
{
	float Distance = FVector::Distance(Location, GetActorLocation());
	
	FVector Direction = FVector(Location - GetActorLocation())/10;
	SetActorLocation(GetActorLocation() + Direction);


	return Distance > Treshold;
}

void AHarvestedPlant::SetThrown(bool value)
{
	Thrown = value;
}

