// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include "Components/StaticMeshComponent.h"
#include "UnnamedCharacter.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Shop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shop Mesh"));
	RootComponent = Shop;

	Shop->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnOverlapBegin);
	Shop->OnComponentEndOverlap.AddDynamic(this, &AShop::OnOverlapEnd);
	


}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AShop::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Personnage = dynamic_cast<AUnnamedCharacter*>(OtherActor);

	if (Personnage)
	{
		Personnage->SetNewCameraOffset(FVector(Personnage->GetDefaultCameraOffset().X, -200, Personnage->GetDefaultCameraOffset().Z));
	}
}

void AShop::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Personnage = dynamic_cast<AUnnamedCharacter*>(OtherActor);

	if (Personnage)
	{
		Personnage->SetNewCameraOffset(Personnage->GetDefaultCameraOffset());
	}
}


