// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/InventoryComponent.h"
#include "Engine/DataTable.h"
#include <Source/Utils/CheckUtils.h>
#include "Source/UI/ItemSlotContainer.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentSize = 0;
	bCanUseInventory = true;
	bIsInventoryOpen = false;
	ItemSlotContainer = nullptr;

	for (int i = 0; i < MaxInventorySize; i++)
	{
		ItemQuantityArr.Add(EMPTY_SLOT_VALUE);
		ItemIdArr.Add(EMPTY_GUID);
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CHECK(ItemSlotContainerClass);

	TObjectPtr<APlayerController> pPlayerController = GetWorld()->GetFirstPlayerController();
	CHECK(pPlayerController);

	ItemSlotContainer = CreateWidget<UItemSlotContainer>(pPlayerController, ItemSlotContainerClass);
	CHECK(ItemSlotContainer);

	ItemSlotContainer->SetVisibility(ESlateVisibility::Hidden);
	ItemSlotContainer->AddToViewport(3);
	ItemSlotContainer->Setup(*this);
}

/// <summary>
/// Finds an empty index in the inventory
/// </summary>
/// <returns></returns>
int UInventoryComponent::FindEmptyIdxInItemArr()
{
	for (int i = 0; i < MaxInventorySize; i++)
	{
		if (ItemQuantityArr[i] == EMPTY_SLOT_VALUE)
		{
			return i;
		}
	}

	return EMPTY_SLOT_VALUE;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/// <summary>
/// Tries to add an item to the inventory, if possible
/// If quantity exceeds a single index, it will try to add it to any empty indexes found
/// </summary>
/// <param name="ItemId"></param>
/// <param name="Quantity"></param>
/// <returns>False if failed to add</returns>
bool UInventoryComponent::TryAdd(const FGuid& ItemId, const int& Quantity)
{
	if (Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// First check if it's possible to add
	int addSpaceAvailable = 0;

	if (ItemIdArr.Contains(ItemId))
	{
		for (int idx : GetIndexesWithItem(ItemId))
		{
			int currQuantity = 0;
			if (TryGetQuantityAtIndex(idx, currQuantity))
			{
				addSpaceAvailable += (MaxItemStackSize - currQuantity);
			}
		}
	}

	for (int idx = 0; idx < MaxInventorySize; idx++)
	{
		if (ItemQuantityArr[idx] == EMPTY_SLOT_VALUE)
		{
			addSpaceAvailable += MaxItemStackSize;
		}
	}

	if (addSpaceAvailable < Quantity)
	{
		return false;
	}

	// Case: Item exists, fill those slots first
	int quantityLeftToAdd = Quantity;
	if (ItemIdArr.Contains(ItemId))
	{
		TArray<int> arrValidIndexes = GetIndexesWithItem(ItemId);

		for (int candidateIdx : arrValidIndexes)
		{
			if (quantityLeftToAdd == 0)
			{
				OnInventoryChange.Broadcast();
				return true;
			}

			int currQty = ItemQuantityArr[candidateIdx];
			int amtCanAdd = MaxItemStackSize - currQty;

			// Slot is filled, can't add any more items
			if (amtCanAdd == 0)
			{
				continue;
			}

			// Slot is not filled, try adding as much as we can
			if (quantityLeftToAdd <= amtCanAdd)
			{
				TryAddAtIndex(ItemId, candidateIdx, quantityLeftToAdd);
				quantityLeftToAdd = 0;
			}
			else
			{
				TryAddAtIndex(ItemId, candidateIdx, amtCanAdd);
				quantityLeftToAdd -= amtCanAdd;
			}
		}
	}

	// Fill in empty spaces if necessary
	while (quantityLeftToAdd != 0)
	{
		// Case Item does not exist, should be able to add anywhere
		int candidateIndex = FindEmptyIdxInItemArr();

		if (quantityLeftToAdd <= MaxItemStackSize) // We have enough, can end here
		{
			TryAddAtIndex(ItemId, candidateIndex, quantityLeftToAdd);
			quantityLeftToAdd = 0;
		}
		else
		{
			TryAddAtIndex(ItemId, candidateIndex, MaxItemStackSize);
			quantityLeftToAdd -= MaxItemStackSize;
		}
	}

	OnInventoryChange.Broadcast();
	return true;

}

/// <summary>
/// Tries to remove an item based on the ItemId.
/// Removes the item from multiple indexes in order if quantity overflows
/// </summary>
/// <param name="ItemId"></param>
/// <param name="Quantity"></param>
/// <returns></returns>
bool UInventoryComponent::TryRemove(const FGuid& ItemId, const int& Quantity)
{
	// Case Item does not exist, quantity is invalid
	if (!ItemIdArr.Contains(ItemId) || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}
	
	TArray<int> candidateIndexes = GetIndexesWithItem(ItemId);

	// first ensure that quantity to remove <= total quantity

	int totalQuantity = 0;

	for (int candidateIdx : candidateIndexes)
	{
		int quantity = 0;

		if (TryGetQuantityAtIndex(candidateIdx, quantity))
		{
			totalQuantity += quantity;
		}
	}

	if (totalQuantity < Quantity)
	{
		// All the item quanitities are still not sufficient, can't remove this item
		return false;
	}

	// Removal is possible
	int leftToRemove = Quantity;

	for (int candidateIdx : candidateIndexes)
	{
		// Finished removal
		if (leftToRemove == 0)
		{
			break;
		}

		int maxRemovedForIdx = 0;

		if (TryGetQuantityAtIndex(candidateIdx, maxRemovedForIdx))
		{
			if (maxRemovedForIdx < leftToRemove)
			{
				// Remove everything at this index
				leftToRemove -= maxRemovedForIdx;
				TryRemoveAtIndex(ItemId, candidateIdx, maxRemovedForIdx);
			}
			else
			{
				// Remove whatever is left, then break out
				TryRemoveAtIndex(ItemId, candidateIdx, leftToRemove);
				leftToRemove = 0;
				break;
			}
		}
	}

	OnInventoryChange.Broadcast();
	return true;

}

/// <summary>
/// Tries to add an item at a particular index
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ArrIdx"></param>
/// <param name="Quantity"></param>
/// <returns></returns>
bool UInventoryComponent::TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	// Case Invalid Index, Large quantity
	if (!ItemIdArr.IsValidIndex(ArrIdx) || Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Invalid Case trying to add to an existing item slot, but the item is different
	if (ItemIdArr[ArrIdx] != ItemId && ItemQuantityArr[ArrIdx] != EMPTY_SLOT_VALUE)
	{
		return false;
	}
	
	// Case adding to an existing item slot, item is same
	if (ItemIdArr[ArrIdx] == ItemId)
	{
		// Invalid Case Quantity exceeds index space
		if (ItemQuantityArr[ArrIdx] + Quantity > MaxItemStackSize)
		{
			return false;
		}

		ItemQuantityArr[ArrIdx] += Quantity;
		OnInventoryChange.Broadcast();
		return true;
	}

	// Case adding to a new index
	CurrentSize++;
	ItemIdArr[ArrIdx] = ItemId;
	ItemQuantityArr[ArrIdx] = Quantity;
	OnInventoryChange.Broadcast();
	return true;
}

/// <summary>
/// 
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ArrIdx"></param>
/// <param name="Quantity"></param>
/// <returns></returns>
bool UInventoryComponent::TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	// Item does not exist, quantity too large/small, invalid arr index
	if (!ItemIdArr.Contains(ItemId) || Quantity > MaxItemStackSize || !ItemQuantityArr.IsValidIndex(ArrIdx) || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Removing index from wrong item, quantity too large
	if (ItemIdArr[ArrIdx] != ItemId || ItemQuantityArr[ArrIdx] - Quantity < 0)
	{
		return false;
	}

	// Remove item from slot scenario
	if (ItemQuantityArr[ArrIdx] - Quantity == 0)
	{
		ItemQuantityArr[ArrIdx] = EMPTY_SLOT_VALUE;
		ItemIdArr[ArrIdx] = EMPTY_GUID;
		OnInventoryChange.Broadcast();
		return true;	
	}

	// Remove, item stacks still remain
	ItemQuantityArr[ArrIdx] -= Quantity;
	OnInventoryChange.Broadcast();
	return true;
}

/// <summary>
/// Tries to get the item data of an item in the inventory
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ResultData"></param>
/// <param name="Quantity"> The total quantity of the item including other indexes</param>
/// <returns></returns>
bool UInventoryComponent::TryGetItem(const FGuid& ItemId, FItemData& ResultData, int& Quantity)
{
	if (!DataTable || !ItemIdArr.Contains(ItemId) || !bCanUseInventory)
	{
		Quantity = EMPTY_SLOT_VALUE;
		return false;
	}

	FString sContextString;
	FName rowName = FName(ItemId.ToString(EGuidFormats::DigitsWithHyphens));
	auto pItemData = DataTable->FindRow<FItemData>(rowName, sContextString);

	if (pItemData && pItemData->Id == ItemId)
	{
		ResultData = *pItemData;

		int totalQuantity = 0;

		// Get the total quantity across all indexes
		TArray<int> validIndexes = GetIndexesWithItem(ItemId);

		for (int& idx : validIndexes)
		{
			int idxQuantity = 0;

			if (TryGetQuantityAtIndex(idx, idxQuantity))
			{
				totalQuantity += idxQuantity;
			}
		}

		Quantity = totalQuantity;

		return true;
	}
	
	return false;
}

/// <summary>
/// Tries to get the item data based on a specific index in the array. 
/// </summary>
/// <param name="ItemArrIdx"></param>
/// <param name="ResultData"></param>
/// <param name="Quantity">The quantity of the item AT THAT INDEX ONLY</param>
/// <returns>False if the target index is invalid or a valid item is not present in that index</returns>
bool UInventoryComponent::TryGetItemAtIndex(int ItemArrIdx, FItemData& ResultData, int& Quantity)
{
	if (!ItemQuantityArr.IsValidIndex(ItemArrIdx) || !ItemIdArr.IsValidIndex(ItemArrIdx))
	{
		return false;
	}

	Quantity = ItemQuantityArr[ItemArrIdx];
	FGuid itemId = ItemIdArr[ItemArrIdx];

	if (Quantity == EMPTY_SLOT_VALUE)
	{
		return false;
	}

	FString sContextString;
	FName rowName = FName(itemId.ToString(EGuidFormats::DigitsWithHyphens));
	auto pItemData = DataTable->FindRow<FItemData>(rowName, sContextString);

	if (!pItemData)
	{
		return false;
	}

	ResultData = *pItemData;

	return true;
}

/// <summary>
/// Increases the size of the inventory
/// Note: Can only increase the inventory size for now
/// </summary>
/// <param name="NewMaxSize">The new inventory size</param>
/// <returns></returns>
bool UInventoryComponent::Resize(const int& NewMaxSize)
{
	if (NewMaxSize <= MaxInventorySize)
	{
		return false;
	}

	for (int i = 0; i < NewMaxSize - MaxInventorySize; i++)
	{
		ItemQuantityArr.Add(EMPTY_SLOT_VALUE);
		ItemIdArr.Add(FGuid::NewGuid());
	}

	OnInventoryChange.Broadcast();
	return true;
}

/// <summary>
/// Attempts to transfer slot values between the same or two different inventories
/// Transfers the entire contents of a slot (all quantity)
/// </summary>
/// <param name="TargetInventory"></param>
/// <param name="SourceSlotIdx"></param>
/// <param name="TargetSlotIdx"></param>
/// <returns>True if operation successful, ofalse otherwise</returns>
bool UInventoryComponent::TryTransferSlots(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& TargetSlotIdx)
{
	if (!TargetInventory || !ItemIdArr.IsValidIndex(SourceSlotIdx) || !ItemQuantityArr.IsValidIndex(SourceSlotIdx))
	{
		return false;
	}

	FGuid itemIdToTransfer = ItemIdArr[SourceSlotIdx];
	int itemQty = ItemQuantityArr[SourceSlotIdx];

	// invalid slot
	if (itemQty == EMPTY_SLOT_VALUE && itemIdToTransfer == EMPTY_GUID)
	{
		return false;
	}

	if (!CanRemoveAtIndex(itemIdToTransfer, SourceSlotIdx, itemQty) ||
		!TargetInventory->CanAddAtIndex(itemIdToTransfer, TargetSlotIdx, itemQty))
	{
		return false;
	}

	TryRemoveAtIndex(itemIdToTransfer, SourceSlotIdx, itemQty);
	TargetInventory->TryAddAtIndex(itemIdToTransfer, TargetSlotIdx, itemQty);

	return true;
}

bool UInventoryComponent::TryTransferSlotsWithQuantity(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& SourceQtyToTransfer, const int& TargetSlotIdx)
{
	if (!TargetInventory || !ItemIdArr.IsValidIndex(SourceSlotIdx) || !ItemQuantityArr.IsValidIndex(SourceSlotIdx))
	{
		return false;
	}

	FGuid itemIdToTransfer = ItemIdArr[SourceSlotIdx];

	// invalid quantity
	if (SourceQtyToTransfer <= 0)
	{
		return false;
	}

	// Verify if possible to remove from src and add to target
	if (!CanRemoveAtIndex(itemIdToTransfer, SourceSlotIdx, SourceQtyToTransfer) ||
		!TargetInventory->CanAddAtIndex(itemIdToTransfer, TargetSlotIdx, SourceQtyToTransfer))
	{
		return false;
	}

	TryRemoveAtIndex(itemIdToTransfer, SourceSlotIdx, SourceQtyToTransfer);
	TargetInventory->TryAddAtIndex(itemIdToTransfer, TargetSlotIdx, SourceQtyToTransfer);

	return true;

}

/// <summary>
/// Verifies if it's possible to remove a particular item & quantity from the index
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ArrIdx"></param>
/// <param name="Quantity"></param>
/// <returns>True if possible, false otherwise</returns>
bool UInventoryComponent::CanRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	// Item does not exist, quantity too large/small, invalid arr index
	if (!ItemIdArr.Contains(ItemId) || Quantity > MaxItemStackSize || !ItemQuantityArr.IsValidIndex(ArrIdx) || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Removing index from wrong item, quantity too large
	if (ItemIdArr[ArrIdx] != ItemId || ItemQuantityArr[ArrIdx] - Quantity < 0)
	{
		return false;
	}

	// Can remove
	return true;
}

/// <summary>
/// Verifies if it's possible to add a particular item & quantity to the index
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ArrIdx"></param>
/// <param name="Quantity"></param>
/// <returns>True if possible, false otherwise</returns>
bool UInventoryComponent::CanAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	// Case Invalid Index, Large quantity
	if (!ItemIdArr.IsValidIndex(ArrIdx) || Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Invalid Case trying to add to an existing item slot, but the item is different
	if (ItemIdArr[ArrIdx] != ItemId && ItemQuantityArr[ArrIdx] != EMPTY_SLOT_VALUE)
	{
		return false;
	}

	// Case adding to an existing item slot, item is same
	if (ItemIdArr[ArrIdx] == ItemId)
	{
		// Invalid Case Quantity exceeds index space
		if (ItemQuantityArr[ArrIdx] + Quantity > MaxItemStackSize)
		{
			return false;
		}

		return true;
	}

	// Case adding to a new index
	return true;
}

/// <summary>
/// Toggles the inventory UI
/// </summary>
void UInventoryComponent::ToggleInventory()
{
	if (bCanUseInventory)
	{
		CHECK(ItemSlotContainer);

		bIsInventoryOpen = !bIsInventoryOpen;
		ESlateVisibility targetVisibility = bIsInventoryOpen ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		ItemSlotContainer->SetVisibility(targetVisibility);

		if (targetVisibility == ESlateVisibility::Visible)
		{
			FInputModeGameAndUI inputMode;
			inputMode.SetWidgetToFocus(ItemSlotContainer->TakeWidget());
			GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		} 
		else if (targetVisibility == ESlateVisibility::Hidden)
		{
			FInputModeGameOnly inputMode;
			GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			ItemSlotContainer->HandleOnCloseSlotContainer();
		}
	}
}

/// <summary>
/// Gets all indexes that contain the item based on item id (guid)
/// </summary>
/// <param name="TargetGuid"></param>
/// <returns></returns>
TArray<int> UInventoryComponent::GetIndexesWithItem(const FGuid& TargetGuid)
{
	TArray<int> arrValidIndexes;

	for (int i = 0; i < ItemIdArr.Num(); i++)
	{
		if (ItemIdArr[i] == TargetGuid)
		{
			arrValidIndexes.Add(i);
		}
	}

	return arrValidIndexes;
}

/// <summary>
/// Gets the first item with the target guid, along with the item information, quantity, and the index it was found
/// </summary>
/// <param name="InTargetGuid"></param>
/// <param name="OutResultData"></param>
/// <param name="OutQuantity"></param>
/// <param name="OutIndex"></param>
/// <returns></returns>
bool UInventoryComponent::TryGetFirstItemWithId(const FGuid& InTargetGuid, FItemData& OutResultData, int& OutQuantity, int& OutIndex)
{
	TArray<int> arrValidIndexes = GetIndexesWithItem(InTargetGuid);

	if (arrValidIndexes.IsEmpty())
	{
		return false;
	}

	int targetItemIdx = arrValidIndexes[0];
	FItemData resultData;
	int qty;

	if (TryGetItemAtIndex(targetItemIdx, resultData, qty))
	{
		OutResultData = resultData;
		OutQuantity = qty;
		OutIndex = targetItemIdx;
		return true;
	}

	return false;
}

/// <summary>
/// Gets a list of all item indexes which have the item with the specified filter
/// </summary>
/// <param name="FilterParams"></param>
/// <returns></returns>
TArray<int> UInventoryComponent::GetIndexesWithItemFilter(const FItemSearchFilterParams& FilterParams)
{
	if (FilterParams.ItemCategory == EItemCategory::None)
	{
		// return all items
		TArray<int> resultItemIdxs;

		for (int i = 0; i < ItemIdArr.Num(); i++)
		{
			if (ItemIdArr[i] != EMPTY_GUID)
			{
				resultItemIdxs.Add(i);
			}
		}

		return resultItemIdxs;
	}

	// Do a search of items with that item category, then check if there are any such items in the inventory
	TArray<int> targetItems;

	for (int i = 0; i < ItemIdArr.Num(); i++)
	{
		FGuid targetGuid = ItemIdArr[i];

		if (targetGuid != EMPTY_GUID)
		{
			// Check the data table for the value, add to target items if it meets the criteria
			FString sContextString;
			FName rowName = FName(targetGuid.ToString(EGuidFormats::DigitsWithHyphens));
			auto pItemData = DataTable->FindRow<FItemData>(rowName, sContextString);

			if (pItemData && pItemData->ItemCategories.Contains(FilterParams.ItemCategory))
			{
				targetItems.Add(i);
			}
		}
	}

	return targetItems;
}

/// <summary>
/// Gets the quantity of an item at the index
/// </summary>
/// <param name="idx"></param>
/// <returns></returns>
bool UInventoryComponent::TryGetQuantityAtIndex(const int& Idx, int& Quantity)
{
	if (!ItemQuantityArr.IsValidIndex(Idx))
	{
		Quantity = EMPTY_SLOT_VALUE;
		return false;
	}

	Quantity = ItemQuantityArr[Idx];

	return Quantity != EMPTY_SLOT_VALUE;
}
