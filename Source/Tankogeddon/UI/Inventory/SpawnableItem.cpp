// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableItem.h"
#include <Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>

#include <Tankogeddon/TankPlayerController.h>

// --------------------------------------------------------------------------------------
void USpawnableItem::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ItemNameText)
	{
		ItemNameText->SetText(ItemName);
	}

	return;
}

// --------------------------------------------------------------------------------------
void USpawnableItem::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->OnLeftMouseButtonUpEvent.AddUObject(this, &USpawnableItem::OnMouseButtonUp);
	}

	return;
}

// --------------------------------------------------------------------------------------
void USpawnableItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SpawnedActor && PlayerController)
	{
		FVector Location, Direction;
		PlayerController->DeprojectMousePositionToWorld(OUT Location, OUT Direction);
		
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams{ FCollisionQueryParams::DefaultQueryParam };
		CollisionParams.AddIgnoredActor(PlayerController->GetPawn());
		CollisionParams.AddIgnoredActor(SpawnedActor);

		const auto TraceDistance{ 100'000.f };
		GetWorld()->LineTraceSingleByChannel(
			OUT HitResult,
			Location,
			Location + Direction * TraceDistance,
			ECollisionChannel::ECC_WorldStatic,
			CollisionParams
		);

		if (HitResult.Actor.Get())
		{
			SpawnedActor->SetActorLocation({ HitResult.Location.X, HitResult.Location.Y, ItemLocationZHint });
			SpawnedActor->SetActorRotation({ 0.f, ItemRotationYHint, 0.f });
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
FReply USpawnableItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		SpawnedActor = GetWorld()->SpawnActor(SpawnableClass);
	}

	return FReply::Handled();
}

// --------------------------------------------------------------------------------------
void USpawnableItem::OnMouseButtonUp()
{
	if (SpawnedActor)
	{
		SpawnedActor = nullptr;
	}
	
	return;
}
