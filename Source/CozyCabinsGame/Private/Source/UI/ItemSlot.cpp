// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemSlot::ClearSlot()
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(nullptr);
	Quantity->SetText(FText::GetEmpty());
	Name->SetText(FText::GetEmpty());
	InventoryIdx = -1;
}

void UItemSlot::SetSlotData(const FItemData& ItemData, const int& Amount, const int& IdxInInventory)
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(ItemData.Icon);
	Quantity->SetText(FText::AsNumber(Amount));
	Name->SetText(FText::FromString(ItemData.Name));

	InventoryIdx = IdxInInventory;
}
