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
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"

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

	// TEST
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

	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	if (HandSocket) 
	{
		auto localTransform = GetMesh()->GetSocketTransform(FName("Hand_LSocket"));
		auto localRotation = localTransform.GetRotation();
		UE_LOG(LogTemp, Warning, TEXT("localRotation %s"), *localRotation.ToString());
	}
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
	PositionX -= 100;
}

void AUnnamedCharacter::MoveDown()
{	
	PositionX += 100;
}


void AUnnamedCharacter::Interact()
{
	auto ItemsInReach = Detector->getOverlappingActors();
	AActor* UsedItem = nullptr;
	float ItemDistance = 1000;

	if (ItemsInReach.Num() > 0) {
		for (const auto* Actor : ItemsInReach)
		{
			if (Actor == nullptr) { return; }
			float Distance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance avec %s: %f"), *Actor->GetName(), Distance);

			if (FVector::Distance(Actor->GetActorLocation(), GetActorLocation()) < ItemDistance)
			{
				ItemDistance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
				UsedItem = const_cast<AActor*>(Actor); // const_cast permet de convertir un const AActor* en AActor*
			}
		}

		if (!UsedItem) { return; }

		// Effectue une rotation du personnage vers la plante � cueillir
		FRotator RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), UsedItem->GetActorLocation());
		AngleRotation = RotationTowardsTarget.Yaw;
		UE_LOG(LogTemp, Warning, TEXT("Rotation %s"), *RotationTowardsTarget.ToString());
		//this->SetActorRotation(FRotator(0, Rotation.Yaw, 0));

		APlantSkeletalMeshActor* Plante = dynamic_cast<APlantSkeletalMeshActor*>(UsedItem);
		if(!Plante)
			UE_LOG(LogTemp, Warning, TEXT("CE N'EST PAS UNE PLANTE !!!!"));

		if (Plante) {
			InteractionTarget = Plante;
			Test();
		}
		else 
		{
			ASol* Sol = dynamic_cast<ASol*>(UsedItem);
			if (Sol) 
			{
				UE_LOG(LogTemp, Warning, TEXT("TEST"));
				Sol->Interact();
			}
		}
	}
}

void AUnnamedCharacter::PickPlants(AActor * Plante)
{
	auto HandSocket = GetMesh()->GetSocketByName(FName("Hand_LSocket"));
	if (HandSocket) {
		auto PlantSocket = Plante->FindComponentByClass<class USkeletalMeshComponent>()->GetSocketByName(FName("Socket"));
		if (PlantSocket) {
			// D�termination de la position local du socket de la plante (Position du socket par rapport � la plante)
			auto PlantSocketLocalTransform = PlantSocket->GetSocketLocalTransform();
			FVector PlantSocketLocalLocation = PlantSocketLocalTransform.GetLocation();

			// D�termination de l'�chelle de la plante
			auto PlantSocketTransform = PlantSocket->GetSocketTransform(Plante->FindComponentByClass<class USkeletalMeshComponent>());
			FVector PlantScale = PlantSocketTransform.GetScale3D();

			// On attache la plante au socket dans la main du personnage
			Plante->K2_AttachRootComponentTo(GetMesh(), FName("Hand_LSocket"), EAttachLocation::SnapToTarget, true);
			Plante->SetActorEnableCollision(false);

			// On d�place repositione la plante dans la main en prenant en compte la position du socket et l'�chelle de la plante
			Plante->SetActorRelativeLocation(-PlantSocketLocalLocation * PlantScale);

			/*
			auto localTransform = GetMesh()->GetSocketTransform(FName("Hand_LSocket"));
			auto localRotation = localTransform.GetRotation();
			UE_LOG(LogTemp, Warning, TEXT("localRotation %s"), *localRotation.ToString());
			*/			
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pas Socket!!"));
		}

		//Plante->SetActorRelativeLocation(-socketOffset);
		/*
		FRotator Rot(Plante->GetRootComponent()->GetRelativeTransform().GetRotation());
		Rot.Yaw -= 90;
		FVector NewOfs = Rot.RotateVector(socketOffset);
		Plante->SetActorRelativeRotation(Rot);
		Plante->SetActorRelativeLocation(NewOfs);
		//dynamic_cast<APlantSkeletalMeshActor*>(Plante)->Interact();
		*/
	}
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

