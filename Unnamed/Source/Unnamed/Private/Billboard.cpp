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

	Disapear();
}

/*
* Fonction ralentissant le temps et déplaçant la caméra au niveau du panneau pour pouvoir le lire.
* Affiche aussi le curseur de la sourie pour pouvoir intéragir avec le HUD.
*/
void ABillboard::ReadBillboard()
{
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
	//FindComponentByClass<UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Inclinaison du panneau pour initier sa chute
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Pitch += 15;
	SetActorRotation(ActorRotation);

	// Initiation de la fonction pour faire disparaitre le panneau
	IsBought = true;
}

/*
* Fonction appelée pour faire disparaitre le panneau une fois que le champs a été acheté
*/
void ABillboard::Disapear()
{
	if (IsBought) 
	{
		DisapearDelay -= GetWorld()->DeltaTimeSeconds;

		if (DisapearDelay < 0)
		{
			if (FindComponentByClass<UStaticMeshComponent>()->IsSimulatingPhysics())
			{
				FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);
				FindComponentByClass<UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			SetActorLocation(GetActorLocation() + FVector(0, 0, -0.2f));

			if (GetActorLocation().Z < 90.0f)
			{
				Destroy();
			}
		}
	}
}

