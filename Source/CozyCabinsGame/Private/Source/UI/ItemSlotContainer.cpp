// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlotContainer.h"
#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Source/Components/InventoryComponent.h"
#include "Components/WrapBox.h"


void UItemSlotContainer::NativeConstruct()
{
	Super::NativeConstruct();

	TotalSlots = 36;
	
}

void UItemSlotContainer::Setup(UInventoryComponent& Inventory)
{
	CHECK(ItemSlotClass);
	CHECK(GridBox);

	InventoryCompRef = &Inventory;
	GridBox->ClearChildren();

	for (int i = 0; i < TotalSlots; i++)
	{
		// Create the object
		TObjectPtr<UItemSlot> pItemSlot = CreateWidget<UItemSlot>(this, ItemSlotClass);
		CHECK(pItemSlot);

		FItemData resultData;
		int quantity;

		// Set slot data if valid quantity
		if (InventoryCompRef->TryGetItemAtIndex(i, resultData, quantity))
		{
			pItemSlot->SetSlotData(resultData, quantity, i);
		}

		GridBox->AddChildToWrapBox(pItemSlot);

		ItemSlots.Add(pItemSlot);
	}
}
