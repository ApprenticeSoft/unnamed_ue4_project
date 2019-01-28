// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnnamedCharacter.generated.h"

class UInteractionDetectorComponent;

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

	// TEST
	virtual void Tick(float DeltaTime) override;
	void DisplacementOnX();
	virtual void BeginPlay() override;
	void MoveUp();
	void MoveDown();

	UFUNCTION(Blueprintcallable)
	void Attraper();

	//void PickUp();
	float XVector = 5.f;
	float PositionX = 0.f;
	UInteractionDetectorComponent* Detector;

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

};
