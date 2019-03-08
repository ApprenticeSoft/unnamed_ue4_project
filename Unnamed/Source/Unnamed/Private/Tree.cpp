// Fill out your copyright notice in the Description page of Project Settings.

#include "Tree.h"
#include "Leaves.h"
#include "MyGameStateBase.h"
#include "Classes/Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATree::ATree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATree::BeginPlay()
{
	Super::BeginPlay();
	
	//GameState = dynamic_cast<AMyGameStateBase*>(GetWorld()->GetGameState());
	CreateLeaves();
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATree::CreateLeaves()
{
	auto SocketArray = FindComponentByClass<class UStaticMeshComponent>()->GetAllSocketNames();
	for (int i = 0; i < SocketArray.Num(); i++)
	{
		auto SocketTransform = FindComponentByClass<class UStaticMeshComponent>()->GetSocketTransform(SocketArray[i]);
		auto Leave = GetWorld()->SpawnActor<ALeaves>(	LeavesBluePrint,
														SocketTransform.GetLocation(),
														UKismetMathLibrary::RandomRotator());

		Leave->SetScale(SocketTransform.GetScale3D().X/GetActorScale3D().X);
	}
}
