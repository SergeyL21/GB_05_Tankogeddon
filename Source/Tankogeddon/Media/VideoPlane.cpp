// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoPlane.h"

#include <MediaPlayer.h>
#include <FileMediaSource.h>
#include <MediaSoundComponent.h>

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

// --------------------------------------------------------------------------------------
// Sets default values
AVideoPlane::AVideoPlane() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AVideoPlane::BeginPlay()
{
	Super::BeginPlay();

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
		MediaPlayer->OpenSource(FileMediaSource);

		auto MediaSound {NewObject<UMediaSoundComponent>(this)};
		MediaSound->SetMediaPlayer(MediaPlayer);
		MediaSound->RegisterComponent();
	}
	
	return;
}

// --------------------------------------------------------------------------------------
// Called every frame
void AVideoPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	return;
}

