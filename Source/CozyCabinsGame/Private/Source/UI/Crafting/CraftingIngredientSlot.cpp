// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Crafting/CraftingIngredientSlot.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Items/ItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UCraftingIngredientSlot::SetupSlot(FCraftingItem ItemData, UInventoryComponent* InventoryComponent, int QuantityMultiplier)
{
	CraftingItemData = ItemData;
	ItemDataStruct = CraftingItemData.ItemReference.DataTable->FindRow<FItemData>(CraftingItemData.ItemReference.RowName, "");
	IMG_ItemIcon->SetBrushFromTexture(ItemDataStruct->Icon);
	TXT_ItemName->SetText(FText::AsCultureInvariant(ItemDataStruct->Name));

	int tempQuantity = 0;
	FItemData tempData;
	FString QuantityText;

	if (InventoryComponent->TryGetItem(ItemDataStruct->Id, tempData, tempQuantity))
	{
		QuantityText = FString::FromInt(tempQuantity) + " / " + FString::FromInt(CraftingItemData.Quantity * QuantityMultiplier);

	}

	else 
	{
		QuantityText = FString::FromInt(0) + " / " + FString::FromInt(CraftingItemData.Quantity * QuantityMultiplier);
	}

	TXT_ItemQuantity->SetText(FText::AsCultureInvariant(QuantityText));
}

void UCraftingIngredientSlot::UpdateSlot(UInventoryComponent* InventoryComponent, int QuantityMultiplier)
{
	int tempQuantity = 0;
	FString QuantityText;
	FItemData tempData;

	if (InventoryComponent->TryGetItem(ItemDataStruct->Id, tempData, tempQuantity))
	{
		QuantityText = FString::FromInt(tempQuantity) + " / " + FString::FromInt(CraftingItemData.Quantity * QuantityMultiplier);
	}

	else 
	{
		QuantityText = FString::FromInt(0) + " / " + FString::FromInt(CraftingItemData.Quantity * QuantityMultiplier);
	}

	TXT_ItemQuantity->SetText(FText::AsCultureInvariant(QuantityText));
}
