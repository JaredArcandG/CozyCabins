// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlotHoverPreview.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include <Source/Utils/CheckUtils.h>

void UItemSlotHoverPreview::SetHoverPreviewSlotData(TObjectPtr<UImage> Image, const FText& ItemName, const FText& ItemDescription)
{
	CHECK(Image);
	CHECK(ItemImage);
	CHECK(ItemNameTextBlock);
	CHECK(ItemDescTextBlock);

	ItemImage->SetBrush(Image->GetBrush());
	ItemNameTextBlock->SetText(ItemName);
	ItemDescTextBlock->SetText(ItemDescription);

}
