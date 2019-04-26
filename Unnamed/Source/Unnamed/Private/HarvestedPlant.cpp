// Fill out your copyright notice in the Description page of Project Settings.

#include "HarvestedPlant.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "UnnamedCharacter.h"
#include "Basket.h"
#include "Point.h"
#include "MyGameStateBase.h"

// Sets default values
AHarvestedPlant::AHarvestedPlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Crop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crop Mesh"));
	RootComponent = Crop;
}

// Called when the game starts or when spawned
void AHarvestedPlant::BeginPlay()
{
	Super::BeginPlay();

	// Si les deux lignes suivantes sont mises dans le constructeur, les collisions ne sont pas prises en compte.
	Crop->OnComponentHit.AddDynamic(this, &AHarvestedPlant::OnCompHit);
	Crop->SetNotifyRigidBodyCollision(true);

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	Basket = dynamic_cast<AUnnamedCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetBasket();

	if (!Basket)
		UE_LOG(LogTemp, Warning, TEXT("Pas de Basket!!!"));

}

// Called every frame
void AHarvestedPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ThrownInBasket();

	Resize();
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

void AHarvestedPlant::ThrownInBasket()
{
	if (Thrown)
	{
		//SetScale(0.3f);
		SetScale(0.3f + 0.7f * (1 + (float)Basket->GetCropNumber())/(float)Basket->GetBasketSize());
		Thrown = MoveToLocation(Basket->GetActorLocation(), 10);
		if (!Thrown)
		{
			Basket->AddCrop(this);
		}
	}
}

void AHarvestedPlant::TriggerDisappear()
{
	if (!IsDisappearing)
		UpdateObjective();
	IsDisappearing = true;
}

void AHarvestedPlant::Resize()
{
	if (IsDisappearing)
	{
		DisappearDelay -= GetWorld()->DeltaTimeSeconds;

		if (DisappearDelay < 0)
		{
			SetActorScale3D(FVector(GetActorScale3D() - GetWorld()->DeltaTimeSeconds));

			if (GetActorScale3D().X < 0) 
			{
				APoint* Point = GetWorld()->SpawnActor<APoint>(	PointBlueprint,
																GetActorLocation(),
																FRotator(0, 0, 0));
				//UpdateObjective();
				if (IsInDemand)
				{
					Point->SetPointValue(3);
					//Point->SetColor(237, 85, 59);
					Point->SetColor(227, 0, 127);
					Point->SetActorScale3D(FVector(1.5, 1.5, 1.3));
				}
				Destroy();
			}
		}
	}
	else
	{
		if (NewScale > Scale)
		{
			Scale += 3 * GetWorld()->DeltaTimeSeconds;
			if (Scale > NewScale)
				Scale = NewScale;
			SetActorScale3D(FVector(Scale));
		}
		else if (NewScale < Scale)
		{
			Scale -= 5 * GetWorld()->DeltaTimeSeconds;
			if (Scale < NewScale)
				Scale = NewScale;
			SetActorScale3D(FVector(Scale));
		}
	}
}

void AHarvestedPlant::SetScale(float value)
{
	NewScale = value;
}

void AHarvestedPlant::UpdateObjective()
{
	if (CropType == ECropType::Corn)
		IsInDemand = GameState->SetCornObjective(GameState->GetCornObjective() - 1);
	else if (CropType == ECropType::Wheat)
		IsInDemand = GameState->SetWheatObjective(GameState->GetWheatObjective() - 1);
	else if (CropType == ECropType::Pumpkin)
		IsInDemand = GameState->SetPumpkinObjective(GameState->GetPumpkinObjective() - 1);
}

void AHarvestedPlant::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(FName("Landscape")))
		TriggerDisappear();
}