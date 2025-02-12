// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Source/Crafting/CraftingData.h"
#include "CraftingIngredientSlot.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UCraftingIngredientSlot : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> HBOX_Main;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemQuantity;

	struct FItemData* ItemDataStruct;
	struct FCraftingItem CraftingItemData;

public:

	void SetupSlot(struct FCraftingItem ItemData, class UInventoryComponent* InventoryComponent, int QuantityMultiplier);
	void UpdateSlot(class UInventoryComponent* InventoryComponent, int QuantityMultiplier);

};
