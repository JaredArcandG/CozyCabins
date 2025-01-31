// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/PlayerInventoryComponent.h"
#include <Source/Utils/CheckUtils.h>

// TODO: Implement this

bool UPlayerInventoryComponent::TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	if (Super::TryAddAtIndex(ItemId, ArrIdx, Quantity))
	{
		// Broadcast an item add event
		SendItemNotification(ItemId, EItemAction::AddItem, Quantity);
		return true;
	}

	return false;
}

bool UPlayerInventoryComponent::TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	if (Super::TryRemoveAtIndex(ItemId, ArrIdx, Quantity))
	{
		// Broadcast an item removal event
		SendItemNotification(ItemId, EItemAction::RemoveItem, Quantity);
		return true;
	}

	return false;
}

void UPlayerInventoryComponent::SendItemNotification(const FGuid& ItemId, const EItemAction& Action, const int& Quantity)
{
	CHECK(DataTable);

	FItemData itemData;

	// Try to send the notification
	if (Super::GetItemData(ItemId, itemData))
	{
		// Broadcast an item add event
		FItemNotification itemNotification;
		itemNotification.ItemAction = Action;
		itemNotification.Quantity = Quantity;
		itemNotification.ItemTexture = itemData.Icon;
		itemNotification.ItemName = FText::FromString(itemData.Name);

		OnPlayerInventoryChange.Broadcast(itemNotification);
	}
}
