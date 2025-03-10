// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Source/Components/StatsComponent.h>
#include "StatsHUD.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API UStatsHUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HungerBar;

	TObjectPtr<class UStatsComponent> ActorStatsRef;

public:

	void InitializeWidget(class APlayerCharacter& PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION(BlueprintCallable)
	void ShowWidget();

protected:

	UFUNCTION()
	void HandleStatsUpdate(EActorStatType UpdatedStat);

	void UpdateHealthBar(float HealthPercent);

	void UpdateStaminaBar(float StaminaPercent);

	void UpdateHungerBar(float HungerPercent);
	
};
