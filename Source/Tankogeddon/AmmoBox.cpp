// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"
#include "TankPawn.h"

// --------------------------------------------------------------------------------------
// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	auto sceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
	RootComponent = sceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

// --------------------------------------------------------------------------------------
void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, 
								AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, 
								int32 OtherBodyIndex, 
								bool bFromSweep, 
								const FHitResult& SweepResult)
{
	auto playerPawn{ Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn()) };
	if (OtherActor == playerPawn)
	{
		playerPawn->SetupCurrentCannon(CannonClass);
		Destroy();
	}
}



