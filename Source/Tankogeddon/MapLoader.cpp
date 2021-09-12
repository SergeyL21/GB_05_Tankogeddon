// Fill out your copyright notice in the Description page of Project Settings. 

#include "MapLoader.h" 
#include <Components/StaticMeshComponent.h> 
#include <Components/PointLightComponent.h> 
#include <Components/BoxComponent.h> 
#include <Kismet/GameplayStatics.h> 
#include <GameFramework/Controller.h> 
#include <Engine/World.h> 

// -------------------------------------------------------------------------------------- 
// Sets default values 
AMapLoader::AMapLoader()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
    PrimaryActorTick.bCanEverTick = false;

    auto SceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
    RootComponent = SceneComponent;

    GatesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
    GatesMesh->SetupAttachment(SceneComponent);

    ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
    ActivatedLight->SetupAttachment(SceneComponent);

    DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
    DeactivatedLight->SetupAttachment(SceneComponent);

    TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
    TriggerCollider->SetupAttachment(SceneComponent);

    TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);

    SetActiveLights();
}

// -------------------------------------------------------------------------------------- 
void AMapLoader::SetIsActivated(bool NewIsActivated)
{
    IsActivated = NewIsActivated;
    SetActiveLights();
    return;
}

// -------------------------------------------------------------------------------------- 
// Called when the game starts or when spawned 
void AMapLoader::BeginPlay()
{
    Super::BeginPlay();

    SetActiveLights();
    return;
}

// -------------------------------------------------------------------------------------- 
void AMapLoader::SetActiveLights()
{
    ActivatedLight->SetHiddenInGame(!IsActivated);
    DeactivatedLight->SetHiddenInGame(IsActivated);
    return;
}

// -------------------------------------------------------------------------------------- 
void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!IsActivated)
    {
        return;
    }

    const auto PlayerPawn{ GetWorld()->GetFirstPlayerController()->GetPawn() };
    if (OtherActor == PlayerPawn)
    {
        UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
    }
    return;
}