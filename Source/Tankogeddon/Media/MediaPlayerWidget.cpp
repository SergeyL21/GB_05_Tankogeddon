// Fill out your copyright notice in the Description page of Project Settings.


#include "MediaPlayerWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <MediaPlayer.h>
#include <MediaTexture.h>
#include <FileMediaSource.h>
#include <MediaSoundComponent.h>

#include <DesktopPlatformModule.h>
#include <IDesktopPlatform.h>

#include <Kismet/KismetRenderingLibrary.h>

// --------------------------------------------------------------------------------------
void UMediaPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BrowseButton)
	{
		BrowseButton->OnClicked.AddDynamic(this, &ThisClass::OnBrowseBtnClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitBtnClicked);
	}

	if (MediaPlayer)
	{
		MediaPlayer->OnMediaOpened.AddDynamic(this, &ThisClass::OnMediaPlayerOpened);
	}
	
	return;
}

// --------------------------------------------------------------------------------------
void UMediaPlayerWidget::OnBrowseBtnClicked()
{
	if (MediaPlayer)
	{
		FString FilePath {};
		if (auto DesktopPlatform = FDesktopPlatformModule::Get())
		{
			auto ParentHandle {FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr)};
			const FString Filter {
				"All Files (*.png, *.jpg, *.mp4)|*.png;*.jpg;*.mp4|"
				"Video Files (*.mp4)|*.mp4|"
				"Image Files (*.png, *.jpg)|*.png;*.jpg"
			};

			TArray<FString> OutFiles;
			DesktopPlatform->OpenFileDialog(
				ParentHandle,
				"Choose media files",
				{},
				{},
				Filter,
				0,
				OutFiles
			);

			if (OutFiles.Num() > 0)
			{
				FilePath = *OutFiles.begin();
			}
		}
		
		auto FileMediaSource {NewObject<UFileMediaSource>()};
		FileMediaSource->FilePath = FilePath;
		if (MediaPlayer->CanPlaySource(FileMediaSource))
		{
			MediaPlayer->OpenSource(FileMediaSource);
		}
		else
		{
			FSlateBrush SlateBrush;
			auto Texture {UKismetRenderingLibrary::ImportFileAsTexture2D(this, FilePath)};
			SlateBrush.SetResourceObject(Texture);
			MediaImage->SetBrush(SlateBrush);
			ResizeKeepAspectRatio(FVector2D(Texture->GetSizeX(), Texture->GetSizeY()));
		}

		auto Pawn {GetWorld()->GetFirstPlayerController()->GetPawn()};
		auto MediaSound {NewObject<UMediaSoundComponent>(Pawn)};
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaSound->RegisterComponent();

		if (MediaImage)
		{
			MediaImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
	}
	
	return;
}

// --------------------------------------------------------------------------------------
void UMediaPlayerWidget::OnExitBtnClicked()
{
	if (MediaPlayer)
	{
		MediaPlayer->Close();
	}

	OnCloseWidget.ExecuteIfBound();

	return;
}

// --------------------------------------------------------------------------------------
void UMediaPlayerWidget::OnMediaPlayerOpened(FString OpenedUrl)
{
	const FVector2D ContentSize {MediaPlayer->GetVideoTrackDimensions(INDEX_NONE, INDEX_NONE)};
	ResizeKeepAspectRatio(ContentSize);

	return;
}

// --------------------------------------------------------------------------------------
void UMediaPlayerWidget::ResizeKeepAspectRatio(const FVector2D& ContentSize)
{
	if (MediaImage)
	{
		const FVector2D ImageSize {MediaImage->GetCachedGeometry().GetLocalSize()};

		const auto ContentAspect {ContentSize.X / ContentSize.Y};
		const auto ImageAspect {ImageSize.X / ImageSize.Y};
		const FVector2D ScaleSize {ImageSize.Y * ContentAspect, ImageSize.Y};
		const auto ScaleFactor {
			ContentAspect > ImageAspect ?
			FVector2D{1.f, FMath::Min(1.f, ImageSize.X / ScaleSize.X) } :
			FVector2D{FMath::Min(1.f, ScaleSize.X / ImageSize.X), 1.f} 
		};

		MediaImage->SetRenderScale(ScaleFactor);
	}

	return;
}

