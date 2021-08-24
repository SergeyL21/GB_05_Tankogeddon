// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

// --------------------------------------------------------------------------------------
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	auto sceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
	RootComponent = sceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
}

// --------------------------------------------------------------------------------------
void AProjectile::Start() 
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
}

// --------------------------------------------------------------------------------------
void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, 
									AActor* OtherActor,
									UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex,
									bool bFromSweep, 
									const FHitResult& SweepResult) 
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
}

// --------------------------------------------------------------------------------------
void AProjectile::Move()
{
	auto nextPosition{ GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate };
	SetActorLocation(nextPosition);
}