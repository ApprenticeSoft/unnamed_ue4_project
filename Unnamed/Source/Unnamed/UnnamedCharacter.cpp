// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UnnamedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionDetectorComponent.h"
#include "PlantSkeletalMeshActor.h"
#include "Sol.h"
#include "Seed.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "WateringCan.h"

AUnnamedCharacter::AUnnamedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnnamedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnnamedCharacter::MoveRight);

	// Personal actions
	PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &AUnnamedCharacter::MoveUp);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &AUnnamedCharacter::MoveDown);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnnamedCharacter::Interact);


	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnnamedCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnnamedCharacter::TouchStopped);
}

void AUnnamedCharacter::BeginPlay()
{
	Super::BeginPlay();

	PositionX = GetActorLocation().X;
	Detector = AActor::FindComponentByClass<class UInteractionDetectorComponent>();

	/*
	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	if (HandSocket) 
	{
		auto localTransform = GetMesh()->GetSocketTransform(FName("Hand_LSocket"));
		auto localRotation = localTransform.GetRotation();
		UE_LOG(LogTemp, Warning, TEXT("localRotation %s"), *localRotation.ToString());
	}
	*/

	// Référence de l'arrosoir
	TArray< AActor* > tempChildActors;
	GetAttachedActors(tempChildActors);
	tempChildActors.FindItemByClass<class AWateringCan>(&Watering_can);
	
	if (!Watering_can)
		UE_LOG(LogTemp, Warning, TEXT("Pas d'arrosoir!!!"));;
}

void AUnnamedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DisplacementOnX();

	if (Detector != nullptr)
	{
		setPossibleInteractionName(Detector->getInteractionName());
		// UE_LOG(LogTemp, Warning, TEXT("Nom de l'acteur: %s"), *getPossibleInteractionName());
	}
}

void AUnnamedCharacter::DisplacementOnX()
{
	if (GetActorLocation().X < PositionX - 10 || GetActorLocation().X  > PositionX + 10) {
		int sign = FMath().Sign(PositionX - GetActorLocation().X);
		float factor = sign * FMath().Square(PositionX - GetActorLocation().X) / 10000;

		AddMovementInput(FVector(factor*XVector, 0.f, 0.f));
	}
}

void AUnnamedCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AUnnamedCharacter::MoveUp()
{
	PositionX -= 150;
}

void AUnnamedCharacter::MoveDown()
{	
	PositionX += 150;
}


void AUnnamedCharacter::Interact()
{
	AActor* UsedItem = nullptr;
	UsedItem = FindTarget();
	if (!UsedItem) { return; }

	ASol* Sol = dynamic_cast<ASol*>(UsedItem);
	if (!Sol) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Ce n'est pas un sol!!"));
		return;
	}

	if (Sol->IsReadyToHarvest())
	{
		SetInteractionTarget(Sol->PopPlant());
		InteractWithPlant();
	}
	else if (Sol->GetSoilState() == ESoilState::Dry)
	{
		SetInteractionTarget(Sol);
		Water();
	}
	else if (Sol->GetPlantNumber() == 0)
	{
		SetInteractionTarget(Sol);
		PlantThePlant(Sol);
		Sow();
	} 
}

void AUnnamedCharacter::PlantThePlant(ASol* Sol)
{
	APlantSkeletalMeshActor* Plant;

	switch (Seed) 
	{
	case ESeed::Corn:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 260, 0));
		break;
	case ESeed::Wheat:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(WheatBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 0, 0));
		UE_LOG(LogTemp, Warning, TEXT("Plant wheat!!!"));
		break;
	case ESeed::Sunflower:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
			FVector(0, 0, -200),
			FRotator(0, 260, 0));
		UE_LOG(LogTemp, Warning, TEXT("Plant Sunflower!!!"));
		break;
	case ESeed::Pumpkin:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
			FVector(0, 0, -200),
			FRotator(0, 260, 0));
		UE_LOG(LogTemp, Warning, TEXT("Plant Pumpkin!!!"));
		break;
	default:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 260, 0));
		break;

	}

	Plant->SetPosition(Sol->GetActorLocation());
	Sol->AddPlant(Plant);
}

void AUnnamedCharacter::ActivateWateringCan()
{
	if (Watering_can)
		Watering_can->Activate();
}

void AUnnamedCharacter::DeactivateWateringCan()
{
	if (Watering_can)
		Watering_can->Deactivate();
}

void AUnnamedCharacter::IncreaseHumidity(float value)
{
	ASol* Sol = dynamic_cast<ASol*>(InteractionTarget);
	if(Sol)
		Sol->SetHumidity(value);
}


AActor* AUnnamedCharacter::FindTarget()
{
	AActor* UsedItem = nullptr;

	auto ItemsInReach = Detector->getOverlappingActors();
	float ItemDistance = 1000;

	if (ItemsInReach.Num() > 0) {
		for (const auto* Actor : ItemsInReach)
		{
			if (Actor == nullptr) { return nullptr; }
			float Distance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance avec %s: %f"), *Actor->GetName(), Distance);

			if (FVector::Distance(Actor->GetActorLocation(), GetActorLocation()) < ItemDistance)
			{
				ItemDistance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
				UsedItem = const_cast<AActor*>(Actor); // const_cast permet de convertir un const AActor* en AActor*
			}
		}
	}

	return UsedItem;
}

void AUnnamedCharacter::SetInteractionTarget(AActor* Target)
{
	if (!Target) { return; }
	InteractionTarget = Target;
	RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), InteractionTarget->GetActorLocation());
	AngleRotation = RotationTowardsTarget.Yaw;
}

void AUnnamedCharacter::PickPlants(AActor * Plante)
{
	if (!Plante) { return; }
	

	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	if (HandSocket) {
		auto PlantSocket = Plante->FindComponentByClass<class USkeletalMeshComponent>()->GetSocketByName(FName("Socket"));
		if (PlantSocket) {
			// Détermination de la position local du socket de la plante (Position du socket par rapport à la plante)
			auto PlantSocketLocalTransform = PlantSocket->GetSocketLocalTransform();
			FVector PlantSocketLocalLocation = PlantSocketLocalTransform.GetLocation();

			// Détermination de l'échelle de la plante
			auto PlantSocketTransform = PlantSocket->GetSocketTransform(Plante->FindComponentByClass<class USkeletalMeshComponent>());
			FVector PlantScale = PlantSocketTransform.GetScale3D();

			// On attache la plante au socket dans la main du personnage
			Plante->K2_AttachRootComponentTo(GetMesh(), FName("Hand_LSocket"), EAttachLocation::SnapToTarget, true);
			Plante->SetActorEnableCollision(false);

			UE_LOG(LogTemp, Warning, TEXT("PlantSocketLocalLocation: %s"), *PlantSocketLocalLocation.ToString());
			UE_LOG(LogTemp, Warning, TEXT("PlantScale: %s"), *PlantScale.ToString());


			// On déplace repositione la plante dans la main en prenant en compte la position du socket et l'échelle de la plante
			//Plante->SetActorRelativeLocation(-PlantSocketLocalLocation * PlantScale);
			Plante->SetActorRelativeLocation(FVector(0, 0, PlantSocketLocalLocation.Y) * PlantScale);
			dynamic_cast<APlantSkeletalMeshActor*>(Plante)->Harvest();
			
			/*
			auto localTransform = GetMesh()->GetSocketTransform(FName("Hand_LSocket"));
			auto localRotation = localTransform.GetRotation();
			UE_LOG(LogTemp, Warning, TEXT("localRotation %s"), *localRotation.ToString());
			*/			
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pas Socket!!"));
		}
	}
}

void AUnnamedCharacter::LaunchSeeds()
{
	// On détermine la position de la main avant de lancer les graines
	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	FVector HandLocation = FVector(0,0,0);
	if (HandSocket)
	{
		auto HandTransform = GetMesh()->GetSocketTransform(FName("Hand_LSocket"));
		HandLocation = HandTransform.GetLocation();
	}

	// On lance les graines
	auto Seed = GetWorld()->SpawnActor<ASeed>(	SeedBluePrint,
												HandLocation,
												GetActorRotation());
	Seed->LaunchSeed(130);
}

void AUnnamedCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AUnnamedCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AUnnamedCharacter::setPossibleInteractionName(FString Name)
{
	PossibleInteractions = Name;
}

FString AUnnamedCharacter::getPossibleInteractionName()
{
	return PossibleInteractions;
}

bool AUnnamedCharacter::MoveToLocation(AActor * Target, float Treshold, bool ColideWithTarget)
{
	if (!Target) { return false; }
	Target->SetActorEnableCollision(ColideWithTarget);

	float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	if (Distance > Treshold) 
		AddMovementInput(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Vector());

	return Distance > Treshold;
}

