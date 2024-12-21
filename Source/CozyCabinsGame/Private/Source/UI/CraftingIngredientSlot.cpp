// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/CraftingIngredientSlot.h"
#include "Source/Crafting/CraftingData.h"
#include "Source/Items/ItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UCraftingIngredientSlot::SetupSlot(FCraftingItem ItemData)
{

	FItemData* Item = ItemData.ItemReference.DataTable->FindRow<FItemData>(ItemData.ItemReference.RowName, "");
	IMG_ItemIcon->SetBrushFromTexture(Item->Icon);
	TXT_ItemName->SetText(FText::AsCultureInvariant(Item->Name));
	TXT_ItemQuantity->SetText(FText::AsCultureInvariant(FString::FromInt(ItemData.Quantity)));
}
