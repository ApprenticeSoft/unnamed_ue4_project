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
#include "Shop.h"
#include "Seed.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "WateringCan.h"
#include "Basket.h"
#include "HarvestedPlant.h"
#include "MyGameStateBase.h"

#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerController.h"

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
	PlayerInputComponent->BindAxis("MoveBackward", this, &AUnnamedCharacter::MoveBackward);

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

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	PositionX = GetActorLocation().X;
	Detector = AActor::FindComponentByClass<class UInteractionDetectorComponent>();

	// Référence de l'arrosoir et au panier
	TArray< AActor* > tempChildActors;
	GetAttachedActors(tempChildActors);
	tempChildActors.FindItemByClass<class AWateringCan>(&Watering_can);
	tempChildActors.FindItemByClass<class ABasket>(&Basket);

	if (!Watering_can)
		UE_LOG(LogTemp, Warning, TEXT("Pas d'arrosoir!!!"));
	
	if (!Basket)
		UE_LOG(LogTemp, Warning, TEXT("Pas de panier!!!"));
	
	// Camera offset
	NormalSocketOffset = CameraBoom->SocketOffset;
	AtShopSocketOffset = FVector(CameraBoom->SocketOffset.X, -200, CameraBoom->SocketOffset.Z);

	//Test
	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Controller->GetViewportSize(ViewportSizeX, ViewportSizeY);
	UE_LOG(LogTemp, Warning, TEXT("ViewportSizeX: %i"), ViewportSizeX);
	UE_LOG(LogTemp, Warning, TEXT("ViewportSizeY: %i"), ViewportSizeY);

	GetScreenToWorldPosition(ViewportSizeX * 0.5f, ViewportSizeY * 0.2f, WorldLocation, WorldDirection);

}

void AUnnamedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DisplacementOnX();

	if (Detector != nullptr)
	{
		if(!IsBusy)
			FindTarget(ClosestTarget);

		// if (dynamic_cast<AShop*>(ClosestTarget))
		if (IsCloseToShop)
		{
			if (SocketOffsetLerpAlpha < 1)
				SocketOffsetLerpAlpha += 2*GetWorld()->DeltaTimeSeconds;
		}
		else
		{
			if (SocketOffsetLerpAlpha > 0)
				SocketOffsetLerpAlpha -= 2*GetWorld()->DeltaTimeSeconds;
		}
		CameraBoom->SocketOffset = FMath::InterpSinInOut(NormalSocketOffset, AtShopSocketOffset, SocketOffsetLerpAlpha);
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

void AUnnamedCharacter::MoveBackward(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}

void AUnnamedCharacter::MoveUp()
{
	PositionX -= 150;
}

void AUnnamedCharacter::MoveDown()
{	
	PositionX += 150;
}

AActor* AUnnamedCharacter::FindTarget(AActor* &ClosestTarget)
{
	if (LastSolTarget)
		LastSolTarget->SetUnselected();
	ClosestTarget = nullptr;

	auto ItemsInReach = Detector->getOverlappingActors();
	float ItemDistance = 1000;

	if (ItemsInReach.Num() > 0) {
		for (const auto* Actor : ItemsInReach)
		{
			if (Actor == nullptr) { return nullptr; }
			float Distance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
			//UE_LOG(LogTemp, Warning, TEXT("Distance avec %s: %f"), *Actor->GetName(), Distance);

			if (FVector::Distance(Actor->GetActorLocation(), GetActorLocation()) < ItemDistance)
			{
				ItemDistance = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());
				ClosestTarget = const_cast<AActor*>(Actor); // const_cast permet de convertir un const AActor* en AActor*
			}
		}
	}

	LastSolTarget = dynamic_cast<ASol*>(ClosestTarget);
	if (LastSolTarget)
	{
		LastSolTarget->SetSelected();
	}

	return ClosestTarget;
}

void AUnnamedCharacter::SetInteractionTarget(AActor* Target)
{
	if (!Target) { return; }
	InteractionTarget = Target;
}

void AUnnamedCharacter::SetRotationAngleTowardsTarget()
{
	RotationTowardsTarget = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), InteractionTarget->GetActorLocation());
	AngleRotation = RotationTowardsTarget.Yaw;
}

/*
 * Fonction appelée quand on appuie sur ESPACE
 * Cette fonction détermine l'action appropriée à effectuée en fonction de la cible de l'intéraction.
 */
void AUnnamedCharacter::Interact()
{
	if (!IsBusy) 
	{
		if (!ClosestTarget) { return; }

		ASol* Sol = dynamic_cast<ASol*>(ClosestTarget);
		if (!Sol)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ce n'est pas un sol!!"));
		}
		else if (Sol->IsReadyToHarvest())
		{ 
			if (Basket->GetCropNumber() < Basket->GetBasketSize())
			{
				SetInteractionTarget(Sol->PopPlant());
				InteractWithPlant();
			}
			else
			{
				NotificationString = "The basket is full !";
				SendNotificationToPlayer();
			}
		}
		else if (Sol->GetSoilState() == ESoilState::Dry)
		{
			SetInteractionTarget(Sol);
			Water();
		}
		else if (Sol->GetPlantNumber() == 0)
		{
			if (CheckIfCanPlantSeed())
			{
				SetInteractionTarget(Sol);
				PlantThePlant(Sol);
				Sow();
			}
			else
			{
				SetInteractionTarget(Sol);
				Water();
			}
		}
		else
		{
			SetInteractionTarget(Sol);
			Water();
		}

		AShop* Shop = dynamic_cast<AShop*>(ClosestTarget);
		if (!Shop)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ce n'est pas un shop!!"));
		}
		else
		{
			SetInteractionTarget(Shop);
			if (Basket->GetCropNumber() > 0)
				InteractWithShop();
			else
			{
				Shop->OpenShop();
				UE_LOG(LogTemp, Warning, TEXT("Magasin!!!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("I'm busy!!!"));
	}
}

void AUnnamedCharacter::SetNotificationString()
{
	NotificationString = SeedString + " can't be planted in " + GameState->GetMonthString();
}

/*
 * Fonction qui ajoute une plante sur la parcelle de sol cible de l'action
 */
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
		break;
	case ESeed::Sunflower:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 260, 0));
		break;
	case ESeed::Pumpkin:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(PumpkinBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 260, 0));
		break;
	default:
		Plant = GetWorld()->SpawnActor<APlantSkeletalMeshActor>(CornBlueprint,
																FVector(0, 0, -200),
																FRotator(0, 260, 0));
		break;

	}

	Plant->SetPosition(Sol->GetActorLocation());
	Plant->SetSol(Sol);
	Sol->AddPlant(Plant);
}

/*
 * Fonction qui vérifie si on peut planter la semence qu'on a choisi durant le mois courrant
 */
bool AUnnamedCharacter::CheckIfCanPlantSeed()
{
	if (Seed == ESeed::Corn)
		return GameState->GetCornSeason();
	else if (Seed == ESeed::Wheat)
		return GameState->GetWheatSeason();
	else if (Seed == ESeed::Pumpkin)
		return GameState->GetPumpkinSeason();
	else
		return false;
}

/*
* Fonction qui fait apparaitre l'arrosoir
*/
void AUnnamedCharacter::ActivateWateringCan()
{
	if (Watering_can)
		Watering_can->Activate();
}

/*
* Fonction qui fait disparaitre l'arrosoir
*/
void AUnnamedCharacter::DeactivateWateringCan()
{
	if (Watering_can)
		Watering_can->Deactivate();
}

/*
* Fonction qui attache le panier à la main du personnage
*/
void AUnnamedCharacter::PutBasketInHand()
{
	Basket->AttachToHand();
}

/*
* Fonction qui attache le panier au dos du personnage
*/
void AUnnamedCharacter::PutBasketOnBack()
{
	Basket->AttachToBack();
}

/*
* Fonction qui appelle la fonction du panier permettant de détacher les légumes qui sont fixés à des sockets du panier.
*/
void AUnnamedCharacter::EmptyBasket()
{
	Basket->ReleaseCrop();
}

/*
* Fonction qui augment l'humidité du sol d'une valeur "value" en une seconde
*/
void AUnnamedCharacter::IncreaseHumidity(float value)
{
	ASol* Sol = dynamic_cast<ASol*>(InteractionTarget);
	if(Sol)
		Sol->SetHumidity(value * GetWorld()->DeltaTimeSeconds);
}

/*
* Fonction qui permet de cueillir une plante et de la fixer au socket de la main
*/
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
			Plante->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hand_LSocket"));
			Plante->SetActorEnableCollision(false);

			// On déplace repositione la plante dans la main en prenant en compte la position du socket et l'échelle de la plante
			Plante->SetActorRelativeLocation(FVector(0, 0, PlantSocketLocalLocation.Y) * PlantScale);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Pas Socket!!"));
		}
	}
}

/*
 * Fonction qui détruit la plante complète dans la main du personnage
 * et la remplace par la version cuellie, plus petite
 */
void AUnnamedCharacter::GetCrop(AActor* Plante)
{
	AHarvestedPlant* Crop;
	Crop = GetWorld()->SpawnActor<AHarvestedPlant>(dynamic_cast<APlantSkeletalMeshActor*>(Plante)->CropBlueprint,
													GetActorLocation(),
													FRotator(0, 260, 0));
	Crop->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hand_LSocket"));
	Crop->SetActorEnableCollision(false);
	InteractionTarget = Crop;
	Plante->Destroy();
}

/*
 * Fonction qui permet de jeter le légume dans le panier
 */
void AUnnamedCharacter::ThrowCrop(AActor* Crop)
{
	dynamic_cast<AHarvestedPlant*>(Crop)->SetThrown(true);
}

/*
 * Fonction qui détermine la position de la main
 * et jette une graine à partir de cette position
 */
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

bool AUnnamedCharacter::PositionNearSoilTarget(AActor * Target, float Treshold, bool ColideWithTarget)
{
	if (!Target) { return false; }
	Target->SetActorEnableCollision(ColideWithTarget);

	float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	if (Distance < Treshold) 
	{
		auto Sign = (FVector(GetActorLocation().X, 0, 0) - FVector(Target->GetActorLocation().X, 0, 0)).GetSignVector();
		AddMovementInput(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation() + FVector(Sign.X*Treshold, 0, 0)).Vector());
	}

	return Distance < Treshold;
}

ABasket* AUnnamedCharacter::GetBasket() const
{
	return Basket;
}

void AUnnamedCharacter::GetScreenToWorldPosition(float ScreenPositionX, float ScreenPositionY, FVector & WorldLocation, FVector & WorldDirection)
{
	Controller->DeprojectScreenPositionToWorld(ScreenPositionX, ScreenPositionY, WorldLocation, WorldDirection);
	UE_LOG(LogTemp, Warning, TEXT("WorldLocation: %s"), *WorldLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("WorldDirection: %s"), *WorldDirection.ToString());
}

FVector2D AUnnamedCharacter::GetScreenSize()
{
	return FVector2D(ViewportSizeX, ViewportSizeY);
}


