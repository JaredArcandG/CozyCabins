// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlotContainer.h"
#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/CanvasPanel.h"

void UItemSlotContainer::NativeConstruct()
{
	Super::NativeConstruct();
	SetupSlots();

	MaxDistanceAllowSlots = 50;
	SlotPaddingX = 10;
	SlotPaddingY = 10;
}

void UItemSlotContainer::SetupSlots()
{
	CHECK(ItemSlotClass);

	int maxRows = FMath::CeilToInt((float)TotalSlots / SlotsPerRow);

	for (int i = 0; i < TotalSlots; i++)
	{
		int rowY = i / SlotsPerRow;
		int rowX = i % SlotsPerRow;

		// Create the object
		TObjectPtr<UItemSlot> pItemSlot = CreateWidget<UItemSlot>(this, ItemSlotClass);
		CHECK(pItemSlot);

		// Calculate the widget location relative to the top left corner of the slot container widget
		int xLoc = SlotPaddingX * rowX;
		int yLoc = SlotPaddingY * rowY;

		FWidgetTransform transform;
		transform.Translation = FVector2D(xLoc, yLoc);

		pItemSlot->SetRenderTransform(transform);

		CHECK(ItemCanvas);
		ItemCanvas->AddChildToCanvas(pItemSlot);

		ItemSlots.Add(pItemSlot);
	}
}

void UItemSlotContainer::GetClosestSlotBasedOnLocation(const FVector2f& ItemLocation, TObjectPtr<UItemSlot>& OutItemSlot)
{
	OutItemSlot = nullptr;

	int targetIndex = -1;
	float targetDist = MaxDistanceAllowSlots;

	for (int i = 0; i < TotalSlots; i++)
	{
		int rowY = i / SlotsPerRow;
		int rowX = i % SlotsPerRow;

		int xLoc = SlotPaddingX * rowX;
		int yLoc = SlotPaddingY * rowY;

		float slotDist = FMath::Sqrt(FMath::Square(yLoc - ItemLocation.Y) + FMath::Square(xLoc - ItemLocation.X));

		if (slotDist <= targetDist)
		{
			targetIndex = i;
			targetDist = slotDist;
		}
	}

	if (ItemSlots.IsValidIndex(targetIndex))
	{
		OutItemSlot = ItemSlots[targetIndex];
	}

}
