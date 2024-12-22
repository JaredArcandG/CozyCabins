// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlotDragPreview.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include <Source/Utils/CheckUtils.h>

void UItemSlotDragPreview::SetPreviewSlotData(const FGuid& Id, const int& Amount, const int& IdxInInventory, UImage& Image)
{
	CHECK(ItemImage);

	ItemId = Id;
	Quantity = Amount;
	ItemImage->SetBrush(Image.GetBrush());
	InventoryIdx = IdxInInventory;

	UE_LOG(LogTemp, Warning, TEXT("Border size set to 80x80."));
}
