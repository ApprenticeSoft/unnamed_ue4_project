// Fill out your copyright notice in the Description page of Project Settings.

#include "Billboard.h"
#include "Classes/Engine/World.h"
#include "UnnamedCharacter.h"
#include "AgriculturalField.h"
#include "MyGameStateBase.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABillboard::ABillboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABillboard::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Parent actor: %s"), *GetParentActor()->GetName());
	
}

// Called every frame
void ABillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABillboard::ReadBillboard()
{
	UE_LOG(LogTemp, Warning, TEXT("Qu'il y a-t-il d'écrit sur ce panneau ?"));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
	AUnnamedCharacter* Player = dynamic_cast<AUnnamedCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->SetNewCameraOffset(Player->GetActorLocation() - GetActorLocation() + FVector(390, 0, -30));
	Player->SetMouseActive(true);
}

int32 ABillboard::GetPrice() const
{
	return Price;
}

void ABillboard::SetPrice(int32 value)
{
	Price = value;
}

void ABillboard::BuyField()
{
	// Achat du champs sur lequel est planté le panneau
	dynamic_cast<AAgriculturalField*>(GetParentActor())->Buy();

	// Soustraction du prix du champs au montant total d'argent
	auto GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());
	GameState->SetPointNumber(GameState->GetPointNumber() - Price);

	// Activation de la simulation physique du panneau 
	FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);

	// Inclinaison du panneau pour initier sa chute
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Pitch += 15;
	SetActorRotation(ActorRotation);
}

