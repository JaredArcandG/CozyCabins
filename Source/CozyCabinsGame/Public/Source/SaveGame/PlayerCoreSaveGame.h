// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "PlayerCoreSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UPlayerCoreSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()

public:

	UPlayerCoreSaveGame();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<FColor> GameImageData;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int GameImageWidth;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int GameImageHeight;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FString GameTime;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FRotator PlayerRotation;

protected:

	UPROPERTY()
	TObjectPtr<UTexture2D> GameImageTexture;

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;

	TObjectPtr<UTexture2D> GetGameImageTexture();

	void LoadGameImageTexture();

private:

	void SaveGameImageTexture(class UTextureRenderTarget2D& InRenderTarget);
	
};
