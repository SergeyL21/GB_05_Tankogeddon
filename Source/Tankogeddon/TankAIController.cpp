// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"

#include <DrawDebugHelpers.h>
#include <Engine/World.h>

// --------------------------------------------------------------------------------------
void ATankAIController::BeginPlay()
{
    Super::BeginPlay();
    TankPawn = Cast<ATankPawn>(GetPawn());
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    MovementAccuracy = TankPawn->GetMovementAccurency();
    const auto Points{ TankPawn->GetPatrollingPoints() };

    const auto PawnLocation{ TankPawn->GetActorLocation() };
    for (const FVector &Point : Points)
    {
        PatrollingPoints.Add(Point + PawnLocation);
    }

    CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
}

// --------------------------------------------------------------------------------------
void ATankAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (CurrentPatrolPointIndex == INDEX_NONE)
    {
        TankPawn->MoveForward(0.f);
        return;
    }

    TankPawn->MoveForward(0.5f);

    const auto RotationValue{ CalculateRotationValue() };
    //UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue: %f"), forwardAngle, rightAngle, rotationValue);
    TankPawn->RotateRight(RotationValue);

    Targeting();
    return;
}

// --------------------------------------------------------------------------------------
float ATankAIController::CalculateRotationValue()
{
    const auto CurrentPoint{ PatrollingPoints[CurrentPatrolPointIndex] };
    const auto PawnLocation{ TankPawn->GetActorLocation() };
    if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccuracy)
    {
        CurrentPatrolPointIndex++;
        if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
        {
            CurrentPatrolPointIndex = 0;
        }
    }

    auto MoveDirection{ CurrentPoint - PawnLocation };
    MoveDirection.Normalize();
    const auto ForwardDirection{ TankPawn->GetActorForwardVector() };
    const auto RightDirection{ TankPawn->GetActorRightVector() };

    DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

    const auto ForwardAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection))) };
    const auto RightAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection))) };

    auto RotationValue{ 0.f };
    if (ForwardAngle > 5.f)
    {
        RotationValue = 1.f;
    }
    if (RightAngle > 90.f)
    {
        RotationValue = -RotationValue;
    }

    return RotationValue;
}

// --------------------------------------------------------------------------------------
void ATankAIController::Targeting()
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
bool ATankAIController::DetectCanFire() const
{
    if (!DetectPlayerVisibility())
    {
        return false;
    }

    const auto TargetingDirection{ TankPawn->GetTurretForwardVector() };
    auto DirectionToPlayer{ PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation() };
    DirectionToPlayer.Normalize();
    const auto AimAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer))) };
    return AimAngle <= Accuracy;
}

// --------------------------------------------------------------------------------------
void ATankAIController::Fire()
{
    TankPawn->Fire();
    return;
}

// --------------------------------------------------------------------------------------
void ATankAIController::RotateToPlayer()
{
    if (IsPlayerInRange())
    {
        TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
    }
    return;
}

// --------------------------------------------------------------------------------------
bool ATankAIController::IsPlayerInRange() const
{
    const auto Distance{ FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) };
    return Distance <= TargetingRange;
}

// --------------------------------------------------------------------------------------
bool ATankAIController::DetectPlayerVisibility() const
{
    auto PlayerPos{ PlayerPawn->GetActorLocation() };
    const auto EyesPos{ TankPawn->GetEyesPosition() };

    FHitResult HitResult;
    auto TraceParams{ FCollisionQueryParams(FName(TEXT("FireTrace")), true, this) };
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(TankPawn);
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