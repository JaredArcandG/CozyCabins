// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACustomPlayerController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UStatsHUD> PlayerHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UStatsHUD> PlayerHUD;

	UPROPERTY();
	TObjectPtr<class UGlobalUIManager> GlobalUIManager;

protected:

	virtual void BeginPlay() override;
	
};
