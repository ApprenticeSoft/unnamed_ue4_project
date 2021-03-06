// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnnamedCharacter.generated.h"

UENUM(BlueprintType)
enum class ESeed : uint8
{
	None, 
	Corn,
	Wheat,
	Pumpkin,
	Sunflower,
	Oat,
	Watermelon
};

class AMyGameStateBase;
class UInteractionDetectorComponent;
class APlayerController;
class APlantSkeletalMeshActor;
class UAnimSequence;
class ASeed;
class AWateringCan;
class ABasket;
class AMachete;
class ASol;

UCLASS(config=Game)
class AUnnamedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	/** Called for side to side input */
	void MoveRight(float Val);
	void MoveBackward(float Val);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void CameraDisplacement();
	void CameraRotation();
	void CameraScroll();
	bool IsCameraScrollable = false;
	void DisplacementOnX();
	void MoveUp();
	void MoveDown();

	AMyGameStateBase* GameState;
	AActor* FindTarget(AActor* &ClosestTarget);
	void SetInteractionTarget(AActor* Target);
	void Interact();
	float XVector = 5.f;
	float PositionX = 0.f;
	FRotator RotationTowardsTarget;
	UInteractionDetectorComponent* Detector = nullptr;
	AWateringCan* Watering_can = nullptr;
	ABasket* Basket = nullptr;
	AMachete* Machete = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	UAnimSequence *MyAnimSequence;

	UFUNCTION(BlueprintCallable)
	void SetRotationAngleTowardsTarget();
	UFUNCTION(BlueprintCallable)
	void PickPlants(AActor * Plante);
	UFUNCTION(BlueprintCallable)
	void GetCrop(AActor * Plante);
	UFUNCTION(BlueprintCallable)
	void ThrowCrop(AActor * Crop);
	UFUNCTION(BlueprintCallable)
	void LaunchSeeds();
	UFUNCTION(BlueprintCallable)
	void ActivateWateringCan();
	UFUNCTION(BlueprintCallable)
	void DeactivateWateringCan();
	UFUNCTION(BlueprintCallable)
	void ActivateMachete();
	UFUNCTION(BlueprintCallable)
	void DeactivateMachete();
	UFUNCTION(BlueprintCallable)
	void PutBasketInHand();
	UFUNCTION(BlueprintCallable)
	void PutBasketOnBack();
	UFUNCTION(BlueprintCallable)
	void EmptyBasket();
	UFUNCTION(BlueprintCallable)
	void IncreaseHumidity(float value = 100.0f);
	UFUNCTION(BlueprintCallable)
	bool CheckIfCanPlantSeed();
	void PlantThePlant(ASol* Sol);
	UPROPERTY(BlueprintReadWrite)
	AActor* InteractionTarget = nullptr;
	AActor* ClosestTarget = nullptr;
	UPROPERTY(BlueprintReadWrite)
	float AngleRotation = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	ESeed Seed = ESeed::Corn;
	UPROPERTY(BlueprintReadWrite)
	FString SeedString = "Corn";
	UPROPERTY(BlueprintReadWrite)
	FString NotificationString = "";
	UFUNCTION(BlueprintCallable)
	void SetNotificationString();
	UPROPERTY(BlueprintReadWrite)
	bool IsBusy = false;
	ASol* LastSolTarget = nullptr;
	float dilation;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	//UClass* ProjectileBluePrint;		// UClass* va afficher la totalit�s des classes disponibles dans le blueprint alors que TSubclassOf<> n'affiche que la classe choisie
	TSubclassOf<ASeed> SeedBluePrint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> CornBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> WheatBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> PumpkinBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> SunflowerBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> OatBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> WatermelonBlueprint;

	bool PossessSelectedCrop = true;
	UPROPERTY(BlueprintReadWrite, Category = "Crops")
	bool PossessSunflower = false;
	UPROPERTY(BlueprintReadWrite, Category = "Crops")
	bool PossessOat = false;
	UPROPERTY(BlueprintReadWrite, Category = "Crops")
	bool PossessWatermelon = false;

	// Position de la camera
	UPROPERTY(BlueprintReadOnly)
	FVector DefaultCameraOffset;
	FVector CurrentCameraOffset;
	FVector NewCameraOffset;
	float SocketOffsetLerpAlpha = 0;

	UPROPERTY(BlueprintReadOnly)
	FRotator DefaultCameraRotation;
	FRotator CurrentCameraRotation;
	FRotator NewCameraRotation;
	float CameraRotationLerpAlpha = 0;

	FVector StartLocation;

	// R�f�rence position sur l'�cran
	APlayerController* Controller;
	int32 ViewportSizeX, ViewportSizeY;
	FVector WorldLocation, WorldDirection;

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AUnnamedCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString PossibleInteractions = "";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsAtShop = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsCloseToShop = false;

	void setPossibleInteractionName(FString Name);
	FString getPossibleInteractionName();

	UFUNCTION(BlueprintCallable)
	void SetMouseActive(bool value);
	UFUNCTION(BlueprintCallable)
	void SetNewCameraOffset(FVector NewOffset);
	UFUNCTION(BlueprintCallable)
	void SetNewCameraRotation(FRotator NewOffset);
	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(AActor* Target, float Treshold = 10.0f, bool ColideWithTarget = false);
	UFUNCTION(BlueprintCallable)
	bool PositionNearSoilTarget(AActor* Target, float Treshold = 120.0f, bool ColideWithTarget = true);
	UFUNCTION(BlueprintCallable)
	void ResetState();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void Sow();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void Water();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void InteractWithPlant();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void InteractWithShop();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void InteractWithBush();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Widget")
	void SendNotificationToPlayer();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Widget")
	void ReadBillboard();
	UFUNCTION(BlueprintCallable, Category = "Widget")
	bool BuyLand();


	FVector GetStartLocation();
	FVector GetDefaultCameraOffset();
	void SetCameraScrollable(bool value);
	void GetScreenToWorldPosition(float ScreenPositionX, float ScreenPositionY, FVector& WorldLocation, FVector& WorldDirection);
	FVector2D GetScreenSize();

	ABasket* GetBasket() const;
};
