// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnnamedCharacter.generated.h"

UENUM(BlueprintType)
enum class ESeed : uint8
{
	Corn,
	Wheat,
	Pumpkin,
	Sunflower
};

class UInteractionDetectorComponent;
class APlantSkeletalMeshActor;
class UAnimSequence;
class ASeed;
class AWateringCan;
class ABasket;
class ASol;
//class APlantSkeletalMeshActor;

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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DisplacementOnX();
	void MoveUp();
	void MoveDown();

	//UFUNCTION(Blueprintcallable)
	void Interact();
	AActor* FindTarget();
	void SetInteractionTarget(AActor* Target);
	float XVector = 5.f;
	float PositionX = 0.f;
	FRotator RotationTowardsTarget;
	UInteractionDetectorComponent* Detector = nullptr;
	AWateringCan* Watering_can = nullptr;
	ABasket* Basket = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
	UAnimSequence *MyAnimSequence;

	UFUNCTION(BlueprintCallable)
	void PickPlants(AActor * Plante);
	UFUNCTION(BlueprintCallable)
	void LaunchSeeds();
	UFUNCTION(BlueprintCallable)
	void ActivateWateringCan();
	UFUNCTION(BlueprintCallable)
	void DeactivateWateringCan();
	UFUNCTION(BlueprintCallable)
	void IncreaseHumidity(float value = 1.0f);
	void PlantThePlant(ASol* Sol);
	UPROPERTY(BlueprintReadWrite)
	AActor* InteractionTarget = nullptr;
	UPROPERTY(BlueprintReadWrite)
	float AngleRotation = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	ESeed Seed = ESeed::Corn;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	//UClass* ProjectileBluePrint;		// UClass* va afficher la totalités des classes disponibles dans le blueprint alors que TSubclassOf<> n'affiche que la classe choisie
	TSubclassOf<ASeed> SeedBluePrint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> CornBlueprint;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<APlantSkeletalMeshActor> WheatBlueprint;

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

	void setPossibleInteractionName(FString Name);
	FString getPossibleInteractionName();

	UFUNCTION(BlueprintCallable)
	bool MoveToLocation(AActor* Target, float Treshold = 10.0f, bool ColideWithTarget = false);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void Sow();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void Water();
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactions")
	void InteractWithPlant();
};
