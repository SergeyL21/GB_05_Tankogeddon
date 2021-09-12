// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include <GameFramework/Pawn.h>
#include "BasePawn.h"
#include <DrawDebugHelpers.h>

// --------------------------------------------------------------------------------------
void ABaseAIController::BeginPlay()
{
    Super::BeginPlay();

    MyPawn = Cast<ABasePawn>(GetPawn());
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!MyPawn || !PlayerPawn)
    {
        MyPawn = Cast<ABasePawn>(GetPawn());
        PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        if (!MyPawn || !PlayerPawn)
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
        RotateToPlayer();
    }

    return;
}

// --------------------------------------------------------------------------------------
void ABaseAIController::RotateToPlayer()
{
    if (IsPlayerInRange())
    {
        MyPawn->SetTurretTarget(PlayerPawn->GetActorLocation());
    }
    return;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::IsPlayerInRange() const
{
    const auto Distance{ FVector::Distance(MyPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) };
    return Distance <= TargetingRange;
}

// --------------------------------------------------------------------------------------
bool ABaseAIController::DetectCanFire() const
{
    if (!DetectPlayerVisibility())
    {
        return false;
    }

    const auto TargetingDirection{ MyPawn->GetTurretForwardVector() };
    auto DirectionToPlayer{ PlayerPawn->GetActorLocation() - MyPawn->GetActorLocation() };
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
bool ABaseAIController::DetectPlayerVisibility() const
{
    auto PlayerPos{ PlayerPawn->GetActorLocation() };
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
            return HitResult.Actor.Get() == PlayerPawn;
        }
    }
    //DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
    return false;
}

