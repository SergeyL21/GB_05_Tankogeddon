// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"

#include <DrawDebugHelpers.h>
#include <Engine/World.h>

// --------------------------------------------------------------------------------------
void ATankAIController::BeginPlay()
{
    Super::BeginPlay();

    TankPawn = Cast<ATankPawn>(BasePawn);
    MovementAccuracy = TankPawn->GetMovementAccurency();
    const auto Points{ TankPawn->GetPatrollingPoints() };

    const auto PawnLocation{ TankPawn->GetActorLocation() };
    for (const FVector &Point : Points)
    {
        PatrollingPoints.Add(Point + PawnLocation);
    }

    CurrentPatrolPointIndex = PatrollingPoints.Num() == 0 ? INDEX_NONE : 0;
    return;
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
    //UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue: %f"), forwardAngle, rightAngle, rotationValue);
    TankPawn->RotateRight(CalculateRotationValue());

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