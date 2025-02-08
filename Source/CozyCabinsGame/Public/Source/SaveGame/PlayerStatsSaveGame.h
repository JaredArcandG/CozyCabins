// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include <Source/Components/StatsComponent.h>
#include "PlayerStatsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UPlayerStatsSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()
	
public:

	UPlayerStatsSaveGame();

	UPROPERTY()
	FActorStats ActorStats;

	UPROPERTY()
	int HungerDecayRate;

	UPROPERTY()
	int HungerDecayGametimeMins;

	UPROPERTY()
	FDateTime TimeSinceLastHungerDecay;

public:

	virtual void OnSave(UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad) override;

};
