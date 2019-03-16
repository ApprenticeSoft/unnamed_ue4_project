// Fill out your copyright notice in the Description page of Project Settings.

#include "Slate_Display.h"
#include "Components/TextRenderComponent.h"
#include "MyGameStateBase.h"

// Sets default values
ASlate_Display::ASlate_Display()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ASlate_Display::BeginPlay()
{
	Super::BeginPlay();

	GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());

	Text = FindComponentByClass<UTextRenderComponent>();

	if (!Text)
		UE_LOG(LogTemp, Warning, TEXT("Pas de text!!!"));

	SetObjective();
}

// Called every frame
void ASlate_Display::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetObjective();
}

void ASlate_Display::SetText(int32 value)
{
	Text->SetText(FText::AsNumber(value));
}

FString ASlate_Display::GetText() const
{
	return Text->Text.ToString();
}

int32 ASlate_Display::GetObjective() const
{
	return Objective;
}

void ASlate_Display::SetObjective()
{
	if(TargetedPlant == ETargetedPlant::Corn)
		Objective = GameState->GetCornObjective();
	else if (TargetedPlant == ETargetedPlant::Wheat)
		Objective = GameState->GetWheatObjective();
	else if (TargetedPlant == ETargetedPlant::Pumpkin)
		Objective = GameState->GetPumpkinObjective();

	SetText(Objective);
}

