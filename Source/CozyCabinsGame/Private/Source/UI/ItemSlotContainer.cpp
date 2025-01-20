// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlotContainer.h"
#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Source/Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include <Source/Player/Controller/CustomPlayerController.h>

/// <summary>
/// Ctor
/// </summary>
void UItemSlotContainer::NativeConstruct()
{
	Super::NativeConstruct();

	TotalSlots = 36;
	bIsFocusable = true;

	CHECK(HoverPreviewWidget);
	HoverPreviewWidget->SetVisibility(ESlateVisibility::Hidden);
}

/// <summary>
/// Sets up the container
/// Should be called after construction
/// </summary>
/// <param name="Inventory"></param>
void UItemSlotContainer::Setup(UInventoryComponent& Inventory)
{
	CHECK(ItemSlotClass);
	CHECK(GridBox);

	InventoryCompRef = &Inventory;
	TotalSlots = InventoryCompRef->MaxInventorySize;
	GridBox->ClearChildren();

	InventoryCompRef->OnInventoryChange.AddDynamic(this, &UItemSlotContainer::UpdateAllSlots);

	for (int i = 0; i < TotalSlots; i++)
	{
		CreateNewItemSlotAddToGrid(i);
	}
}

/// <summary>
/// Updates all the slots in the inventory UI
/// Can be called each time the inventory is invoked, or any time the inventory changes
/// </summary>
void UItemSlotContainer::UpdateAllSlots()
{
	CHECK(ItemSlotClass);
	CHECK(GridBox);
	CHECK(InventoryCompRef);

	// Update existing slots to the appropriate value in the Inventory, add new slots if inventory expanded
	TotalSlots = InventoryCompRef->MaxInventorySize;
	ItemSlots.Empty();

	for (int i = 0; i < TotalSlots; i++)
	{
		TObjectPtr<UWidget> pWidget = GridBox->GetChildAt(i);

		if (TObjectPtr<UItemSlot> pItemSlot = Cast<UItemSlot>(pWidget))
		{
			FItemData resultData;
			int quantity;

			// Set slot data if valid quantity
			if (!InventoryCompRef->TryGetItemAtIndex(i, resultData, quantity))
			{
				pItemSlot->SetEmptySlot(i, *InventoryCompRef);
				ItemSlots.Add(pItemSlot);
				continue;
			}
			
			pItemSlot->SetSlotData(resultData, quantity, i, *InventoryCompRef);
			ItemSlots.Add(pItemSlot);
			continue;
		}

		// Otherwise we need to create a new widget 
		CreateNewItemSlotAddToGrid(i);
	}
}

/// <summary>
/// Make sure item slots are properly closed
/// Dragged previews should disapper
/// </summary>
void UItemSlotContainer::HandleOnCloseSlotContainer()
{
	for (auto pItemSlot : ItemSlots)
	{
		if (pItemSlot)
		{
			pItemSlot->HandleOnClose();
		}
	}
}

/// <summary>
/// Creates a new item slot widget and adds it to the grid
/// Can be used on initial setup to create the widget, or during an upgrade if the inventory size changed
/// </summary>
/// <param name="ItemIdx"></param>
void UItemSlotContainer::CreateNewItemSlotAddToGrid(const int& ItemIdx)
{
	CHECK(GridBox);
	CHECK(InventoryCompRef);

	// Create the object
	TObjectPtr<UItemSlot> pItemSlot = CreateWidget<UItemSlot>(this, ItemSlotClass);
	CHECK(pItemSlot);

	FItemData resultData;
	int quantity;

	// Set slot data if valid quantity
	if (InventoryCompRef->TryGetItemAtIndex(ItemIdx, resultData, quantity))
	{
		pItemSlot->SetSlotData(resultData, quantity, ItemIdx, *InventoryCompRef);
	}
	else
	{
		pItemSlot->SetEmptySlot(ItemIdx, *InventoryCompRef);
	}

	// Subscribe to hover preview event in slot
	pItemSlot->OnEnterHoverPreview.AddUniqueDynamic(this, &UItemSlotContainer::ShowHoverPreviewWidget);
	pItemSlot->OnExitHoverPreview.AddUniqueDynamic(this, &UItemSlotContainer::HideHoverPreviewWidget);

	GridBox->AddChildToWrapBox(pItemSlot);

	ItemSlots.Add(pItemSlot);
}

/// <summary>
/// Shows the hover preview widget with the appropriate data
/// </summary>
/// <param name="HoverPreviewData"></param>
void UItemSlotContainer::ShowHoverPreviewWidget(const FHoverPreviewData HoverPreviewData)
{
	CHECK(HoverPreviewWidget);

	HoverPreviewWidget->SetHoverPreviewSlotData(HoverPreviewData.Image, HoverPreviewData.ItemName, HoverPreviewData.ItemDescription);
	HoverPreviewWidget->SetVisibility(ESlateVisibility::Visible);
}

/// <summary>
/// Hides the hover preview widget
/// </summary>
void UItemSlotContainer::HideHoverPreviewWidget()
{
	CHECK(HoverPreviewWidget);

	HoverPreviewWidget->SetVisibility(ESlateVisibility::Hidden);
}
