// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Items/ItemSlotDragPreview.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/Sizebox.h"
#include "Components/TextBlock.h"
#include <Source/Items/Item.h>

/// <summary>
/// Sets the preview widget's data
/// </summary>
/// <param name="Id"></param>
/// <param name="Amount"></param>
/// <param name="IdxInInventory"></param>
/// <param name="Image"></param>
void UItemSlotDragPreview::SetPreviewSlotData(const FGuid& Id, const int& Amount, const int& IdxInInventory, TObjectPtr<UImage> Image, TSubclassOf<AItem> InItemClass, const bool& IsDroppable)
{
	CHECK(ItemImage);
	CHECK(ItemText);
	CHECK(Image);
	CHECK(InItemClass);

	ItemId = Id;
	Quantity = Amount;
	ItemImage->SetBrush(Image->GetBrush());
	InventoryIdx = IdxInInventory;
	ItemText->SetText(FText::AsNumber(Amount));
	ItemClass = InItemClass;
	bIsDroppable = IsDroppable;
}
