// Fill out your copyright notice in the Description page of Project Settings.

#include "AgriculturalField.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAgriculturalField::AAgriculturalField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	CustomMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Component"));
	//CustomMeshComponent->AttachToComponent(RootComponent);
	CustomMeshComponent->SetupAttachment(RootComponent);
	CustomMeshComponent->OnBeginCursorOver.AddDynamic(this, &AAgriculturalField::CustomOnBeginMouseOver);
	*/
}

// Called when the game starts or when spawned
void AAgriculturalField::BeginPlay()
{
	Super::BeginPlay();

	GetAllChildActors(Children);

	if (!IsOwned)
		for (AActor* Child : Children)
			Child->SetActorEnableCollision(false);
}

// Called every frame
void AAgriculturalField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAgriculturalField::SetOwned(bool value)
{
	IsOwned = value;
}

bool AAgriculturalField::GetOwned()
{
	return IsOwned;
}

void AAgriculturalField::Buy()
{
	IsOwned = true;

	for (AActor* Child : Children)
		Child->SetActorEnableCollision(true);
}

void AAgriculturalField::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Over !!!!"));
}

