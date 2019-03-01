// Fill out your copyright notice in the Description page of Project Settings.

#include "Point.h"
#include "Classes/Components/TextRenderComponent.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/PlayerController.h"

// Sets default values
APoint::APoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void APoint::BeginPlay()
{
	Super::BeginPlay();
	
	
	Text = FindComponentByClass<UTextRenderComponent>();
	
	if (!Text)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pas de UTextRenderComponent!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTextRenderComponent présent!"));
	}

	Text->SetText(FString::FromInt(PointValue));
	
}

// Called every frame
void APoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), 10);

}

int32 APoint::GetPointValue()
{
	return PointValue;
}

void APoint::SetPointValue(int32 value)
{
	PointValue = value;
}

bool APoint::MoveToLocation(FVector Location, float Treshold)
{
	float Distance = FVector::Distance(Location, GetActorLocation());

	FVector Direction = FVector(Location - GetActorLocation()) / 10;
	SetActorLocation(GetActorLocation() + Direction);

	return Distance > Treshold;
}

