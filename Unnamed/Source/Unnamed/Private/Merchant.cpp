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
	{
		UE_LOG(LogTemp, Warning, TEXT("Pas de CornSlate!!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CornSlate: %s"), *CornSlate->GetFullName());
	}


}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if(CornSlate)
		MoveToLocation(CornSlate);
	*/

	if (!IsBusy)
	{
		CheckCornObjective();
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

void AMerchant::CheckCornObjective()
{
	if (GameState->GetCornObjective() != CornSlate->GetObjective())
	{
		InteractionTarget = CornSlate;
		UpdateSlate();

		/*
		if (!MoveToLocation(CornSlate, 70))
		{
			UE_LOG(LogTemp, Warning, TEXT("FINI"));
			UpdateSlate();
		}
		*/
	}
}

void AMerchant::PickSlate(ASlate_Display * Slate)
{
	if (!Slate) { return; }

	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	if (HandSocket) {
		auto SlateSocket = Slate->FindComponentByClass<class UStaticMeshComponent>()->GetSocketByName(FName("Socket"));
		if (SlateSocket) {
			// Détermination de la position local du socket de l'ardoise (Position du socket par rapport à la plante)
			
			FTransform SlateSocketTransform;
			SlateSocket->GetSocketTransform(SlateSocketTransform, Slate->FindComponentByClass<class UStaticMeshComponent>());
			FVector SlateSocketLocation = SlateSocketTransform.GetLocation();

			// Détermination de l'échelle de l'ardoise
			FVector SlateScale = SlateSocketTransform.GetScale3D();

			UE_LOG(LogTemp, Warning, TEXT("SlateSocketLocation: %s"), *SlateSocketLocation.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Slate->GetActorLocation(): %s"), *Slate->GetActorLocation().ToString());
			//UE_LOG(LogTemp, Warning, TEXT("Offset: %s"), *Offset.ToString());


			// On attache la plante au socket dans la main du personnage
			Slate->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hand_LSocket"));
			Slate->SetActorEnableCollision(false);


			auto Offset = SlateSocketLocation - Slate->GetActorLocation();
			// On déplace repositione la plante dans la main en prenant en compte la position du socket et l'échelle de la plante
			//Slate->SetActorRelativeLocation(FVector(0, 0, SlateSocketLocation.Y) * SlateScale);
			Slate->SetActorRelativeLocation(Offset);
			Slate->SetActorRelativeRotation(FRotator(180,0,0));
			
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pas Socket!!"));
		}
	}
}

