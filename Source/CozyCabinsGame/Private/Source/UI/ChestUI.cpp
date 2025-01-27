// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ChestUI.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/Components/InventoryComponent.h>
#include <Source/UI/ItemSlotContainer.h>

void UChestUI::Setup(UInventoryComponent& PlayerInventory, UInventoryComponent& ChestInventory)
{
	CHECK(PlayerItemSlotContainer);
	CHECK(ChestItemSlotContainer);

	PlayerItemSlotContainer->Setup(PlayerInventory);
	ChestItemSlotContainer->Setup(ChestInventory);

}

void UChestUI::UpdateUI()
{
	PlayerItemSlotContainer->UpdateAllSlots();
	ChestItemSlotContainer->UpdateAllSlots();
}
