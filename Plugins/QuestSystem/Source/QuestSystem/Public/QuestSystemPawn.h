// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include "InteractableObject.h"
#include "QuestSystemPawn.generated.h"

class UQuestList;
class UQuestDialog;

UCLASS()
class QUESTSYSTEM_API AQuestSystemPawn : public APawn, public IInteractableObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestDialog> QuestDialogClass;

public:
	// Sets default values for this character's properties
	AQuestSystemPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact_Implementation(AActor* ActorInteractedWithObject) override;

private:
	/* bool PawnHasQuest {false}; */
};
