// Fill out your copyright notice in the Description page of Project Settings.

#include "Basket.h"
#include "PlantSkeletalMeshActor.h"
#include "Classes/Engine/World.h"
#include "HarvestedPlant.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/GameFramework/Pawn.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ABasket::ABasket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasket::BeginPlay()
{
	Super::BeginPlay();
	
	SocketArray = FindComponentByClass<class UStaticMeshComponent>()->GetAllSocketNames();
	UE_LOG(LogTemp, Warning, TEXT("SocketArray: %i"), SocketArray.Num());


	PlayerSkeletalMesh = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<class USkeletalMeshComponent>();
	auto ChestSocket = PlayerSkeletalMesh->GetSocketByName(FName("ChestSocket"));
	BackRelativeTransform = GetTransform().GetRelativeTransform(ChestSocket->GetSocketTransform(PlayerSkeletalMesh));

	//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

}

// Called every frame
void ABasket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsReleasingCrops) 
	{
		/*
		CropReleaseDelay--;

		if (CropReleaseDelay < 0)
		{
			if (ReleaseIndex > -1)
			{
				UE_LOG(LogTemp, Warning, TEXT("ReleaseIndex: %i"), ReleaseIndex);
				HarvestedPlants[ReleaseIndex]->SetActorLocation(HarvestedPlants[ReleaseIndex]->GetActorLocation() + GetActorRotation().RotateVector(FVector(0, 0, 25.0f)));

				HarvestedPlants[ReleaseIndex]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				HarvestedPlants[ReleaseIndex]->SetActorEnableCollision(true);
				HarvestedPlants[ReleaseIndex]->FindComponentByClass<class UStaticMeshComponent>()->SetSimulatePhysics(true);
				HarvestedPlants[ReleaseIndex]->FindComponentByClass<class UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HarvestedPlants.RemoveAt(ReleaseIndex);
				SocketIndex--;

				ReleaseIndex--;
			}

			CropReleaseDelay = 10;
		}
		*/
		
		for (int32 i = HarvestedPlants.Num()-1; i > -1; i--)
		{
			// Sécurité au cas où plusieurs plantes dépaces la limite en même temps.
			//if (i > HarvestedPlants.Num() - 1) i = HarvestedPlants.Num() - 1;

			HarvestedPlants[i]->SetActorLocation(HarvestedPlants[i]->GetActorLocation() + GetActorRotation().RotateVector(FVector(0,0,1.8f)));
			auto Distance = FVector::Distance(HarvestedPlants[i]->GetActorLocation(), GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);

			if (Distance > ReleaseDistance)
			{
				HarvestedPlants[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				HarvestedPlants[i]->SetActorEnableCollision(true);
				HarvestedPlants[i]->FindComponentByClass<class UStaticMeshComponent>()->SetSimulatePhysics(true);
				//HarvestedPlants[i]->LaunchCrop(GetActorRotation().RotateVector(FVector(0, 0, 1)), 180);
				HarvestedPlants[i]->FindComponentByClass<class UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				HarvestedPlants.RemoveAt(i);
				SocketIndex--;
				ReleaseDistance += 3;
			}
		}
		
		if (HarvestedPlants.Num() == 0)
		{
			IsReleasingCrops = false;
			ReleaseDistance = 50;
		}
		
	}

}

void ABasket::AttachToBack()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachToComponent(GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<class USkeletalMeshComponent>(),
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName("ChestSocket")
	);
	SetActorRelativeTransform(BackRelativeTransform);
}

void ABasket::AttachToHand()
{
	auto HandSocket = PlayerSkeletalMesh->GetSocketByName(FName("Finger_04_LSocket"));
	HandRelativeTransform = GetTransform().GetRelativeTransform(HandSocket->GetSocketTransform(PlayerSkeletalMesh));

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachToComponent(GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<class USkeletalMeshComponent>(),
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		FName("Finger_04_LSocket")
	);
	SetActorRelativeTransform(HandRelativeTransform);
}

void ABasket::AddCrop(APlantSkeletalMeshActor* Crop)
{
	AHarvestedPlant* plant;

	switch (Crop->GetPlantType())
	{
	case EPlantType::Corn:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedCornBlueprint,
														FVector(0, 0, 100),
														FRotator(0, 260, 0));
		break;
	case EPlantType::Wheat:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedWheatBlueprint,
														GetActorLocation(),
														FRotator(0, 260, 0));
		break;
	default:
		plant = GetWorld()->SpawnActor<AHarvestedPlant>(HarvestedCornBlueprint,
														FVector(0, 0, 100),
														FRotator(0, 260, 0));
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Nombre de récolte: %i"), HarvestedPlants.Num());
}

void ABasket::AddCrop(AHarvestedPlant * Crop)
{
	if (SocketIndex < SocketArray.Num())
	{
		HarvestedPlants.Add(Crop);
		Crop->AttachToComponent(FindComponentByClass<class UStaticMeshComponent>(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketArray[SocketIndex]);
		SocketIndex += 1;
	}
}

int32 ABasket::GetCropNumber()
{
	return HarvestedPlants.Num();
}

void ABasket::ReleaseCrop()
{
	IsReleasingCrops = true;
	ReleaseIndex = HarvestedPlants.Num() - 1;
}


