// Fill out your copyright notice in the Description page of Project Settings.

#include "Point.h"
#include "Classes/Components/TextRenderComponent.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"
#include "UnnamedCharacter.h"
#include "MyGameStateBase.h"

// Sets default values
APoint::APoint()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APoint::BeginPlay()
{
	Super::BeginPlay();
	
	
	Text = FindComponentByClass<UTextRenderComponent>();
	
	if (!Text)
		UE_LOG(LogTemp, Warning, TEXT("Pas de UTextRenderComponent!"));

	//Text->SetText(FString::FromInt(PointValue));
	Text->SetText(FText::AsNumber(PointValue));
	SetColor(17, 47, 65);
	Character = dynamic_cast<AUnnamedCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void APoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveToLocation(Character->GetActorLocation(), 10)) 
		AddPoints();

}

int32 APoint::GetPointValue()
{
	return PointValue;
}

void APoint::SetPointValue(int32 value)
{
	PointValue = value;
	Text->SetText(FText::AsNumber(PointValue));
}

void APoint::SetColor(uint8 R, uint8 G, uint8 B)
{
	Text->SetTextRenderColor(FColor(R, G, B));
}

bool APoint::MoveToLocation(FVector Location, float Treshold)
{
	MoveDelay -= GetWorld()->DeltaTimeSeconds;

	float Distance = FVector::Distance(Location, GetActorLocation());
	if (MoveDelay < 0)
	{
		FVector Direction = FVector(Location - GetActorLocation()) / 10;
		SetActorLocation(GetActorLocation() + Direction);
	}
	
	return Distance > Treshold;
}

void APoint::AddPoints()
{
	auto GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());
	GameState->SetCornNumber(GameState->GetCornNumber() + PointValue);
	Destroy();
}

