// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include <GameFramework/Pawn.h>
#include "BasePawn.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>

// --------------------------------------------------------------------------------------
void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();

    MyPawn = Cast<ABasePawn>(GetPawn());

	return;
}

// --------------------------------------------------------------------------------------
ABasePawn* ABaseAIController::GetCurrentEnemy() const
{
    return EnemyPawn;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::SetCurrentEnemy(ABasePawn* CurrentEnemy)
{
    if (!EnemyPawn)
    {
        EnemyPawn = CurrentEnemy;
    }
    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::ResetCurrentEnemy()
{
    EnemyPawn = nullptr;
    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MyPawn)
    {
        MyPawn = Cast<ABasePawn>(GetPawn());
        if (!MyPawn)
        {
            return;
        }
    }

    if (DetectCanFire())
    {
        Fire();
    }
    else
    {
        RotateToEnemy();
    }

    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::RotateToEnemy()
{
    if (EnemyPawn)
    {
        MyPawn->SetTurretTarget(EnemyPawn->GetActorLocation());
    }
    return;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::DetectCanFire() const
{
    if (!DetectEnemyVisibility())
    {
        return false;
    }

    const auto TargetingDirection{ MyPawn->GetTurretForwardVector() };
    auto DirectionToPlayer{ EnemyPawn->GetActorLocation() - MyPawn->GetActorLocation() };
    DirectionToPlayer.Normalize();
    const auto AimAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer))) };
    return AimAngle <= Accuracy;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Fire()
{
    MyPawn->Fire();
    return;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::DetectEnemyVisibility() const
{
    if (!EnemyPawn)
    {
        return false;
    }

    auto PlayerPos{ EnemyPawn->GetActorLocation() };
    const auto EyesPos{ MyPawn->GetEyesPosition() };

    FHitResult HitResult;
    auto TraceParams{ FCollisionQueryParams(FName(TEXT("FireTrace")), true, this) };
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(MyPawn);
    TraceParams.bReturnPhysicalMaterial = false;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
    {
        if (HitResult.Actor.Get())
        {
            /*if (HitResult.Actor.Get() == PlayerPawn)
            {
                DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Yellow, false, 0.5f, 0, 10);
            }
            else
            {
                DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
            }*/
            return HitResult.Actor.Get() == EnemyPawn;
        }
    }
    //DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
    return false;
}

