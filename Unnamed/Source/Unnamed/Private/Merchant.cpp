// Fill out your copyright notice in the Description page of Project Settings.

#include "Merchant.h"
#include "Classes/Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "MyGameStateBase.h"
#include "Slate_Display.h"
#include "Point.h"

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

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	if (!CornSlate)
		UE_LOG(LogTemp, Warning, TEXT("Pas de CornSlate!!"));
	if (!WheatSlate)
		UE_LOG(LogTemp, Warning, TEXT("Pas de WheatSlate!!"));
	if (!PumpkinSlate)
		UE_LOG(LogTemp, Warning, TEXT("Pas de PumpkinSlate!!"));

	StartLocation = GetActorLocation();
}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsBusy)
	{
		CheckObjective();

		RandomMovement();
	}
}

// Called to bind functionality to input
void AMerchant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMerchant::RandomMovement()
{
	RandomMovementDelay -= GetWorld()->DeltaTimeSeconds;

	if (RandomMovementDelay < 0)
	{
		RandomMovementDelay = UKismetMathLibrary::RandomFloatInRange(5.0f, 20.0f);
		NewLocation = StartLocation + FVector(UKismetMathLibrary::RandomIntegerInRange(0, 100), UKismetMathLibrary::RandomIntegerInRange(0, 100), 0);
		Move();
	}
}

bool AMerchant::MoveToTarget(AActor * Target, float Treshold, bool ColideWithTarget)
{
	if (!Target) { return false; }
	Target->SetActorEnableCollision(ColideWithTarget);

	float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	if (Distance > Treshold)
		AddMovementInput(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Vector());

	return Distance > Treshold;
}

bool AMerchant::MoveToLocation(FVector Location, float Treshold)
{
	float Distance = FVector::Distance(Location, GetActorLocation());
	if (Distance > Treshold)
		AddMovementInput(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location).Vector());

	return Distance > Treshold;
}

void AMerchant::CheckObjective()
{
	if (GameState->GetCornObjective() != CornSlate->GetObjective())
	{
		InteractionTarget = CornSlate;
		UpdateSlate();
	}
	else if (GameState->GetWheatObjective() != WheatSlate->GetObjective())
	{
		InteractionTarget = WheatSlate;
		UpdateSlate();
	}
	else if (GameState->GetPumpkinObjective() != PumpkinSlate->GetObjective())
	{
		InteractionTarget = PumpkinSlate;
		UpdateSlate();
	}

}

void AMerchant::PickSlate(ASlate_Display * Slate)
{
	if (!Slate) { return; }

	// Position de la main
	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	FTransform HandSocketTransform = HandSocket->GetSocketTransform(GetMesh());
	FVector HandSocketLocation = HandSocketTransform.GetLocation();

	// Position de l'ardoise
	auto SlateSocket = Slate->FindComponentByClass<class UStaticMeshComponent>()->GetSocketByName(FName("Socket"));
	FTransform SlateSocketTransform;
	SlateSocket->GetSocketTransform(SlateSocketTransform, Slate->FindComponentByClass<class UStaticMeshComponent>());
	FVector SlateSocketLocation = SlateSocketTransform.GetLocation();

	// Position relative de l'ardoise par rapport à la main
	FVector Offset = HandSocketLocation - SlateSocketLocation;
	FTransform HandRelativeTransform = Slate->GetTransform().GetRelativeTransform(HandSocketTransform);

	//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Slate->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName("Hand_LSocket")
	);
	Slate->SetActorRelativeTransform(HandRelativeTransform);
	Slate->AddActorWorldOffset(Offset);
}

void AMerchant::PutSlateBack(ASlate_Display * Slate)
{
	if (!Slate) { return; }
	Slate->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Slate->SetActorLocation(Slate->GetDefaultLocation());

	if (Slate->GetObjective() == 0)
		SpawnPoints(10, FVector(249, 244, 0), 1.6, Slate->GetActorLocation());
		//SpawnPoints(10, FVector(242, 177, 52), 1.6, Slate->GetActorLocation());
}

void AMerchant::UpdateSlateText(ASlate_Display * Slate)
{
	if (!Slate) { return; }
	Slate->SetObjective();
}

void AMerchant::SpawnPoints(int32 value, FVector Color, float scale, FVector Location)
{
	APoint* Point = GetWorld()->SpawnActor<APoint>(	PointBlueprint,
													Location,
													FRotator(0, 0, 0));
	Point->SetPointValue(value);
	Point->SetColor(Color.X, Color.Y, Color.Z);
	Point->SetActorScale3D(FVector(scale));
}

