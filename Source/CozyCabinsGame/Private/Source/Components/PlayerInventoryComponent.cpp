// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/PlayerInventoryComponent.h"
#include <Source/Utils/CheckUtils.h>

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	bDisableNotification = false;
}

bool UPlayerInventoryComponent::TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	if (Super::TryAddAtIndex(ItemId, ArrIdx, Quantity))
	{
		// Broadcast an item add event
		if (!bDisableNotification)
		{
			SendItemNotification(ItemId, EItemAction::AddItem, Quantity);
		}

		return true;
	}

	return false;
}

bool UPlayerInventoryComponent::TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	if (Super::TryRemoveAtIndex(ItemId, ArrIdx, Quantity))
	{
		// Broadcast an item removal event
		if (!bDisableNotification)
		{
			SendItemNotification(ItemId, EItemAction::RemoveItem, Quantity);
		}

		return true;
	}

	return false;
}

bool UPlayerInventoryComponent::TryTransferSlots(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& TargetSlotIdx)
{
	if (TargetInventory == this)
	{
		bDisableNotification = true;
	}

	bool bResult = Super::TryTransferSlots(TargetInventory, SourceSlotIdx, TargetSlotIdx);

	bDisableNotification = false;

	return bResult;
}

bool UPlayerInventoryComponent::TryTransferSlotsWithQuantity(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& SourceQtyToTransfer, const int& TargetSlotIdx)
{
	if (TargetInventory == this)
	{
		bDisableNotification = true;
	}

	bool bResult = Super::TryTransferSlotsWithQuantity(TargetInventory, SourceSlotIdx, SourceQtyToTransfer, TargetSlotIdx);

	bDisableNotification = false;

	return bResult;
}

void UPlayerInventoryComponent::SendItemNotification(const FGuid& ItemId, const EItemAction& Action, const int& Quantity)
{
	CHECK(DataTable);

	FItemData itemData;

	// Try to send the notification
	if (Super::GetItemData(ItemId, itemData))
	{
		auto pNotification = NewObject<UItemNotification>(this);
		CHECK(pNotification);

		pNotification->Setup(ItemWidgetNotificationClass);
		pNotification->ItemAction = Action;
		pNotification->ItemTexture = itemData.Icon;
		pNotification->Quantity = Quantity;
		pNotification->ItemName = FText::FromString(itemData.Name);

		// Broadcast an inventory item change event UI notification

		OnPlayerInventoryChange.Broadcast(pNotification);
	}
}
