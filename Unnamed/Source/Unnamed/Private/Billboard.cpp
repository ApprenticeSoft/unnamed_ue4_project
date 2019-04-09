// Fill out your copyright notice in the Description page of Project Settings.

#include "Billboard.h"
#include "Classes/Engine/World.h"
#include "UnnamedCharacter.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/Kismet/GameplayStatics.h"

// Sets default values
ABillboard::ABillboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABillboard::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Parent actor: %s"), *GetParentActor()->GetName());
	
}

// Called every frame
void ABillboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABillboard::ReadBillboard()
{
	UE_LOG(LogTemp, Warning, TEXT("Qu'il y a-t-il d'écrit sur ce panneau ?"));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
	AUnnamedCharacter* Player = dynamic_cast<AUnnamedCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->SetNewCameraOffset(Player->GetActorLocation() - GetActorLocation() + FVector(390, 0, -30));
	Player->SetMouseActive(true);
}

