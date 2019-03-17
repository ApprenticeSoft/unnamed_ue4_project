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


}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsBusy)
	{
		CheckObjective();
	}
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
}

void AMerchant::UpdateSlateText(ASlate_Display * Slate)
{
	if (!Slate) { return; }
	Slate->SetObjective();
}

