// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include <GameFramework/Pawn.h>
#include "BasePawn.h"
#include <DrawDebugHelpers.h>

// --------------------------------------------------------------------------------------
void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();

    BasePawn = Cast<ABasePawn>(GetPawn());
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Targeting()
{
    if (DetectCanFire())
    {
        Fire();
    }
    else
    {
        RotateToPlayer();
    }
    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::RotateToPlayer()
{
    if (IsPlayerInRange())
    {
        BasePawn->RotateTurretTo(PlayerPawn->GetActorLocation());
    }
    return;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::IsPlayerInRange() const
{
    const auto Distance{ FVector::Distance(BasePawn->GetActorLocation(), PlayerPawn->GetActorLocation()) };
    return Distance <= TargetingRange;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::DetectCanFire() const
{
    if (!DetectPlayerVisibility())
    {
        return false;
    }

    const auto TargetingDirection{ BasePawn->GetTurretForwardVector() };
    auto DirectionToPlayer{ PlayerPawn->GetActorLocation() - BasePawn->GetActorLocation() };
    DirectionToPlayer.Normalize();
    const auto AimAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer))) };
    return AimAngle <= Accuracy;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Fire()
{
    BasePawn->Fire();
    return;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::DetectPlayerVisibility() const
{
    auto PlayerPos{ PlayerPawn->GetActorLocation() };
    const auto EyesPos{ BasePawn->GetEyesPosition() };

    FHitResult HitResult;
    auto TraceParams{ FCollisionQueryParams(FName(TEXT("FireTrace")), true, this) };
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(BasePawn);
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
            return HitResult.Actor.Get() == PlayerPawn;
        }
    }
    //DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
    return false;
}
