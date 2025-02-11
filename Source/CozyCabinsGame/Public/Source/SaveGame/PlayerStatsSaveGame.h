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

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FActorStats ActorStats;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int HungerDecayRate;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int HungerDecayGametimeMins;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FDateTime TimeSinceLastHungerDecay;

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;

};
