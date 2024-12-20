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

	// -1 represents empty space
	for (int i = 0; i < MaxInventorySize; i++)
	{
		ItemQuantityArr.Add(-1);
		ItemIdArr.Add(FGuid::NewGuid());
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
	ItemSlotContainer->AddToViewport();
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
		if (ItemQuantityArr[i] == -1)
		{
			return i;
		}
	}

	return -1;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/// <summary>
/// Tries to add an item to the inventory, if possible
/// </summary>
/// <param name="ItemId"></param>
/// <param name="Quantity"></param>
/// <returns>False if failed to add</returns>
bool UInventoryComponent::TryAdd(const FGuid& ItemId, const int& Quantity)
{
	if (Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Case: Item exists, needs to be added
	if (ItemIdArr.Contains(ItemId))
	{
		TArray<int> arrValidIndexes = GetIndexesWithItem(ItemId);

		for (int candidateIdx : arrValidIndexes)
		{
			if (TryAddAtIndex(ItemId, candidateIdx, Quantity))
			{
				return true;
			}  
		}

		// Item needs to be added in a new index

		// No space, can't add
		if (CurrentSize == MaxInventorySize || Quantity > MaxItemStackSize)
		{
			return false;
		}
		
		// Should be able to add
		int candidateIdx = FindEmptyIdxInItemArr();

		// For some reason, item arr does not match up and we couldn't find a valid index
		if (candidateIdx == -1)
		{
			return false;
		}

		return TryAddAtIndex(ItemId, candidateIdx, Quantity);
	}

	// Case: Item does not exist, No more space in the inventory
	if (!ItemIdArr.Contains(ItemId) && CurrentSize == MaxInventorySize)
	{
		return false;
	}

	// Case Item does not exist, should be able to add
	int candidateIndex = FindEmptyIdxInItemArr();

	if (candidateIndex == -1)
	{
		return false;
	}

	ItemQuantityArr[candidateIndex] = Quantity;
	ItemIdArr[candidateIndex] = ItemId;
	CurrentSize++;
	return true;

}

/// <summary>
/// Tries to remove an item based on the ItemId.
/// Removes the first item it can find in the inventory that matches, if one exists
/// </summary>
/// <param name="ItemId"></param>
/// <param name="Quantity"></param>
/// <returns></returns>
bool UInventoryComponent::TryRemove(const FGuid& ItemId, const int& Quantity)
{
	// Case Item does not exist, quantity is invalid
	if (!ItemIdArr.Contains(ItemId) || Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}
	
	int candidateIdx = GetIndexesWithItem(ItemId)[0];

	return TryRemoveAtIndex(ItemId, candidateIdx, Quantity);

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
	
	// Case adding new item 
	if (!ItemIdArr.Contains(ItemId))
	{
		// Different item exists in that index
		if (ItemQuantityArr[ArrIdx] != -1)
		{
			return false;
		}

		ItemQuantityArr[ArrIdx] = Quantity;
		ItemIdArr[ArrIdx] = ItemId;
		CurrentSize++;
		return true;
	}

	// Case addint to existing idx
	if (ItemQuantityArr[ArrIdx] + Quantity > MaxItemStackSize)
	{
		return false;
	}

	ItemQuantityArr[ArrIdx] = Quantity;
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
		ItemQuantityArr[ArrIdx] = -1;
		ItemIdArr[ArrIdx] = FGuid::NewGuid();

		return true;	
	}

	// Remove, item stacks still remain
	ItemQuantityArr[ArrIdx] -= Quantity;
	return true;
}

/// <summary>
/// Tries to get the item data of an item in the inventory
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ResultData"></param>
/// <returns></returns>
bool UInventoryComponent::TryGetItem(const FGuid& ItemId, FItemData& ResultData)
{
	if (!DataTable || !ItemIdArr.Contains(ItemId) || !bCanUseInventory)
	{
		return false;
	}

	FString sContextString;
	auto pItemData = DataTable->FindRow<FItemData>(FName(ItemId.ToString()), sContextString);

	if (pItemData && pItemData->Id == ItemId)
	{
		ResultData = *pItemData;
		return true;
	}
	
	return false;
}

/// <summary>
/// Tries to get the item data based on a specific index in the array. 
/// </summary>
/// <param name="ItemArrIdx"></param>
/// <param name="ResultData"></param>
/// <returns>False if the target index is invalid or a valid item is not present in that index</returns>
bool UInventoryComponent::TryGetItemAtIndex(int ItemArrIdx, FItemData& ResultData, int& Quantity)
{
	if (!ItemQuantityArr.IsValidIndex(ItemArrIdx))
	{
		return false;
	}

	Quantity = ItemQuantityArr[ItemArrIdx];

	if (Quantity == -1)
	{
		return false;
	}

	return TryGetItem(ItemIdArr[ItemArrIdx], ResultData);
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
		ItemQuantityArr.Add(-1);
		ItemIdArr.Add(FGuid::NewGuid());
	}

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
