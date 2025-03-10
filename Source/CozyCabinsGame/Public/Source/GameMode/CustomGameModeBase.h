// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API ACustomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACustomGameModeBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UDataTable> ItemsDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UGameTimeManager> GameTimeManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings", meta = (ClampMin = "0.0"))	
	float DroppedItemTimeToLiveSeconds;
	
};
