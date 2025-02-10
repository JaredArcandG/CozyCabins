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
	FVector PlayerLocation;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FRotator PlayerRotation;

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;
	
};
