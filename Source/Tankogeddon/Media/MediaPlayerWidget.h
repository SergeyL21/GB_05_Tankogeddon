// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MediaTexture.h"
#include "MediaPlayerWidget.generated.h"

class UButton;
class UImage;
class UMediaPlayer;
class UMediaTexture;

DECLARE_DYNAMIC_DELEGATE(FOnCloseWidget);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMediaPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCloseWidget OnCloseWidget;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BrowseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	UImage *MediaImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMediaPlayer *MediaPlayer;

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void OnBrowseBtnClicked();

	UFUNCTION()
	void OnExitBtnClicked();

	UFUNCTION()
	void OnMediaPlayerOpened(FString OpenedUrl);

private:
	void ResizeKeepAspectRatio(const FVector2D &ContentSize);
	
};
