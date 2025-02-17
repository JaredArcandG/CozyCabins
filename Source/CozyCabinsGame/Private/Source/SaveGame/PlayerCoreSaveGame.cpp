// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/PlayerCoreSaveGame.h"
#include <Source/Player/PlayerCharacter.h>
#include <Source/Utils/CheckUtils.h>
#include <Kismet/GameplayStatics.h>
#include <Source/Utils/CommonUtils.h>
#include "Blueprint/UserWidget.h"
#include <ImageUtils.h>
#include <Components/Image.h>
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Camera/CameraComponent.h"

UPlayerCoreSaveGame::UPlayerCoreSaveGame()
{
	PlayerLocation = FVector::Zero();
	PlayerRotation = FRotator();
	GameTime = FString();
	GameImageTexture = nullptr;
}

void UPlayerCoreSaveGame::OnSave(const UObject& WorldContextObject, UObject& ObjectToSave)
{
	CHECK(WorldContextObject.GetWorld());
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(&ObjectToSave);
	CHECK(pPlayer);
	CHECK(pPlayer->FollowCamera);

	PlayerLocation = pPlayer->GetActorLocation();
	PlayerRotation = pPlayer->GetActorRotation();

	// Save the current time in accordance to local times
	FDateTime currentTime = FDateTime::Now();
	GameTime = currentTime.ToString();

	// Take a snapshot of the current game, compress it into an image
	TArray<TObjectPtr<ASceneCapture2D>> arrSceneCaptures = UCommonUtils::GetAllActorsInWorld<ASceneCapture2D>(*WorldContextObject.GetWorld());

	if (arrSceneCaptures.Num() > 0)
	{
		TObjectPtr<ASceneCapture2D> pSceneCapture = arrSceneCaptures[0];

		if (pSceneCapture && pSceneCapture->GetCaptureComponent2D())
		{
			pSceneCapture->SetActorLocation(pPlayer->FollowCamera->GetComponentLocation());
			pSceneCapture->SetActorRotation(pPlayer->FollowCamera->GetComponentRotation());
			pSceneCapture->GetCaptureComponent2D()->CaptureScene();
			SaveGameImageTexture(*pSceneCapture->GetCaptureComponent2D()->TextureTarget);
		}
	}
}

void UPlayerCoreSaveGame::OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(&ObjectToLoad);
	CHECK(pPlayer);

	pPlayer->SetActorLocation(PlayerLocation);
	pPlayer->SetActorRotation(PlayerRotation);

}

TObjectPtr<UTexture2D> UPlayerCoreSaveGame::GetGameImageTexture()
{
	return GameImageTexture;
}

void UPlayerCoreSaveGame::SaveGameImageTexture(UTextureRenderTarget2D& InRenderTarget)
{
	GameImageData.Empty();

	FTextureRenderTargetResource* pRenderTargetResource = InRenderTarget.GameThread_GetRenderTargetResource();
	GameImageWidth = InRenderTarget.SizeX;
	GameImageHeight = InRenderTarget.SizeY;

	if (pRenderTargetResource)
	{
		FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);
		pRenderTargetResource->ReadPixels(GameImageData, readPixelFlags);
		UE_LOG(LogTemp, Log, TEXT("Save -> Expected data size: %d"), GameImageWidth * GameImageHeight);
		UE_LOG(LogTemp, Log, TEXT("Save -> Actual data size: %d"), GameImageData.Num());

		GameImageTexture = UTexture2D::CreateTransient(InRenderTarget.SizeX, InRenderTarget.SizeY);
		void* pTextureData = GameImageTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(pTextureData, GameImageData.GetData(), GameImageData.Num() * sizeof(FColor));
		GameImageTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		GameImageTexture->UpdateResource();
	}

}

void UPlayerCoreSaveGame::LoadGameImageTexture()
{
	GameImageTexture = UTexture2D::CreateTransient(GameImageWidth, GameImageHeight);

	if (GameImageTexture && !GameImageData.IsEmpty())
	{
		void* pTextureData = GameImageTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
		UE_LOG(LogTemp, Log, TEXT("Load -> Expected data size: %d"), GameImageWidth * GameImageHeight);
		UE_LOG(LogTemp, Log, TEXT("Load -> Actual data size: %d"), GameImageData.Num());
		FMemory::Memcpy(pTextureData, GameImageData.GetData(), GameImageData.Num() * sizeof(FColor));
		GameImageTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		GameImageTexture->UpdateResource();
	}

}
