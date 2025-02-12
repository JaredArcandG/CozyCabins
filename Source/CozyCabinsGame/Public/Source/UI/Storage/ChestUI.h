// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestUI.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UChestUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UItemSlotContainer> PlayerItemSlotContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UItemSlotContainer> ChestItemSlotContainer;

public:

	void Setup(class UInventoryComponent& PlayerInventory, class UInventoryComponent& ChestInventory);

	void UpdateUI();
	
};
