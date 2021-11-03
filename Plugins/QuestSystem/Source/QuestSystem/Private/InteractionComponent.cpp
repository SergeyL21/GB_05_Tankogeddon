// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "InteractableObject.h"
#include <Blueprint/UserWidget.h>

// --------------------------------------------------------------------------------------
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionCompBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionCompEndOverlap);

	return;
}

// --------------------------------------------------------------------------------------
void UInteractionComponent::Interact()
{
	if (ActorToInteract)
	{
		// for blueprints
		IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
	}

	return;
}

// --------------------------------------------------------------------------------------
void UInteractionComponent::OnInteractionCompBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UInteractableObject>())
	{
		ActorToInteract = OtherActor;
		if (!InteractMessage)
		{
			if (InteractMessageClass)
			{
				InteractMessage = CreateWidget<UUserWidget>(GetWorld(), InteractMessageClass);
			}
		}

		if (InteractMessage && !InteractMessage->IsInViewport())
		{
			InteractMessage->AddToViewport();
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void UInteractionComponent::OnInteractionCompEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ActorToInteract = nullptr;
	if (InteractMessage)
	{
		InteractMessage->RemoveFromViewport();
	}
	
	return;
}
