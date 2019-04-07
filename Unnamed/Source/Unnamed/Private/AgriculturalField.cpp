// Fill out your copyright notice in the Description page of Project Settings.

#include "AgriculturalField.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Sol.h"

// Sets default values
AAgriculturalField::AAgriculturalField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	CustomMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Component"));
	RootComponent = CustomMeshComponent;
	CustomMeshComponent->OnBeginCursorOver.AddDynamic(this, &AAgriculturalField::CustomOnBeginMouseOver);
	CustomMeshComponent->OnEndCursorOver.AddDynamic(this, &AAgriculturalField::CustomOnEndMouseOver);	
	*/
}

// Called when the game starts or when spawned
void AAgriculturalField::BeginPlay()
{
	Super::BeginPlay();
	//CustomMeshComponent->OnClicked.AddDynamic(this, &AAgriculturalField::CustomOnClick);

	GetAllChildActors(Children);

	if (!IsOwned)
		for (AActor* Child : Children)
		{
			if(dynamic_cast<ASol*>(Child))
				Child->SetActorEnableCollision(false);
		}

	UE_LOG(LogTemp, Warning, TEXT("Children.Num(): %i"), Children.Num());
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
	for (AActor* Child : Children)
		dynamic_cast<ASol*>(Child)->Highlight(true);
}

void AAgriculturalField::CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("End Mouse Over !!!!"));
	for (AActor* Child : Children)
		dynamic_cast<ASol*>(Child)->Highlight(false);
}

void AAgriculturalField::CustomOnClick(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Click C++"));
}

