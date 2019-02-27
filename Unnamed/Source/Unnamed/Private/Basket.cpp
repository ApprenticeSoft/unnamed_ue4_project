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

	UE_LOG(LogTemp, Warning, TEXT("Nombre de r�colte: %i"), HarvestedPlants.Num());
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

void ABasket::ReleaseCrop()
{
	UE_LOG(LogTemp, Warning, TEXT("Legumes dans le panier: %i"), HarvestedPlants.Num());
	for (AHarvestedPlant* crop : HarvestedPlants) 
	{
		crop->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		crop->SetActorEnableCollision(true);
		crop->FindComponentByClass<class UStaticMeshComponent>()->SetSimulatePhysics(true);
		crop->FindComponentByClass<class UStaticMeshComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UE_LOG(LogTemp, Warning, TEXT("Crop name: %s"), *crop->GetFName().ToString());
	}
	HarvestedPlants.Empty();
	SocketIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("Legumes dans le panier: %i"), HarvestedPlants.Num());
}


