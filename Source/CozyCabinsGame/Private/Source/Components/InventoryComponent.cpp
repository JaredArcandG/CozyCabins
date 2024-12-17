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
		ItemArr.Add(-1);
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
}

int UInventoryComponent::FindEmptyIdxInItemArr()
{
	for (int i = 0; i < MaxInventorySize; i++)
	{
		if (ItemArr[i] == -1)
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

bool UInventoryComponent::TryAdd(const FGuid& ItemId, const int& Quantity)
{
	if (Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Case: Item exists, needs to be added
	if (ItemCacheMap.Contains(ItemId))
	{
		TArray<int> arrValidIndexes = ItemCacheMap[ItemId];

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
	if (!ItemCacheMap.Contains(ItemId) && CurrentSize == MaxInventorySize)
	{
		return false;
	}

	// Case Item does not exist, should be able to add
	int candidateIndex = FindEmptyIdxInItemArr();

	if (candidateIndex == -1)
	{
		return false;
	}

	ItemArr[candidateIndex] = 1;
	TTuple<FGuid, TArray<int>> kvp = MakeTuple(ItemId, TArray<int>{ candidateIndex });
	ItemCacheMap.Add(kvp);
	CurrentSize++;
	return true;

}

bool UInventoryComponent::TryRemove(const FGuid& ItemId, const int& Quantity)
{
	// Case Item does not exist, quantity is invalid
	if (!ItemCacheMap.Contains(ItemId) || Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}
	
	int arrIdx = ItemCacheMap[ItemId].Num() - 1;

	if (arrIdx == -1)
	{
		return false;
	}

	int candidateIdx = ItemCacheMap[ItemId][arrIdx];

	return TryRemoveAtIndex(ItemId, candidateIdx, Quantity);

}

bool UInventoryComponent::TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity)
{
	// Case Invalid Index, Large quantity
	if (!ItemArr.IsValidIndex(ArrIdx) || Quantity > MaxItemStackSize || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}
	
	// Case adding new item 
	if (!ItemCacheMap.Contains(ItemId))
	{
		// Different item exists in that index
		if (ItemArr[ArrIdx] != -1)
		{
			return false;
		}

		ItemArr[ArrIdx] = Quantity;
		TTuple<FGuid, TArray<int>> kvp = MakeTuple(ItemId, TArray<int>{ ArrIdx });
		ItemCacheMap.Add(kvp);
		CurrentSize++;
		return true;
	}

	// Case addint to existing idx
	if (ItemArr[ArrIdx] + Quantity > MaxItemStackSize)
	{
		return false;
	}

	ItemArr[ArrIdx] = Quantity;
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
	if (!ItemCacheMap.Contains(ItemId) || Quantity > MaxItemStackSize || !ItemArr.IsValidIndex(ArrIdx) || Quantity <= 0 || !bCanUseInventory)
	{
		return false;
	}

	// Removing index from wrong item, quantity too large
	if (!ItemCacheMap[ItemId].Contains(ArrIdx) || ItemArr[ArrIdx] - Quantity < 0)
	{
		return false;
	}

	// Remove item from slot scenario
	if (ItemArr[ArrIdx] - Quantity == 0)
	{
		ItemArr[ArrIdx] = -1;
		ItemCacheMap[ItemId].Remove(ArrIdx);

		// Remove key if no more slots with this item left
		if (ItemCacheMap[ItemId].Num() == 0)
		{
			ItemCacheMap.Remove(ItemId);
			CurrentSize--;
			return true;
		}

		return true;	
	}

	// Remove, item stacks still remain
	ItemArr[ArrIdx] -= Quantity;
	return true;
}

/// <summary>
/// Tries to get the item data of an item in the inventory
/// </summary>
/// <param name="ItemId"></param>
/// <param name="ResultData"></param>
/// <returns></returns>
bool UInventoryComponent::TryGetItem(const FGuid& ItemId, FItemData ResultData)
{
	if (!DataTable || !ItemCacheMap.Contains(ItemId) || !bCanUseInventory)
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
		ItemArr.Add(-1);
	}

	return true;
}

void UInventoryComponent::ToggleInventory()
{
	if (bCanUseInventory)
	{
		CHECK(ItemSlotContainer);

		bIsInventoryOpen = !bIsInventoryOpen;
		ItemSlotContainer->SetVisibility(bIsInventoryOpen ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}
