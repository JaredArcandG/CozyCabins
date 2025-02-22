// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> DeathScreenImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> OnDeath;


	void OnPlayerDeath(UObject& ObjectToBind, const FName& FunctionToBind);

	void ResetWidget();
};
