// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "GameTimeSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UGameTimeSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()

public:

	UGameTimeSaveGame();

	UPROPERTY()
	FDateTime CurrentGameTime;

public:

	virtual void OnSave(UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad) override;
	
};
