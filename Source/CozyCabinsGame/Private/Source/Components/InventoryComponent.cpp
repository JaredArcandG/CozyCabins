// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CurrentSize = 0;

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

bool UInventoryComponent::TryAddAtIndex(const FGuid& ItemId)
{

	// Case: Item exists, needs to be added
	if (ItemCacheMap.Contains(ItemId))
	{
		TArray<int> arrValidIndexes = ItemCacheMap[ItemId];

		for (int candidateIdx : arrValidIndexes)
		{
			if (!ItemArr.IsValidIndex(candidateIdx))
			{
				continue;
			}

			if (ItemArr[candidateIdx] >= MaxItemStackSize)
			{
				continue;
			}

			ItemArr[candidateIdx]++;
			return true;

		}

		// Item needs to be added in a new index

		// No space, can't add
		if (CurrentSize == MaxInventorySize)
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

		ItemArr[candidateIdx] = 1;
		ItemCacheMap[ItemId].Add(candidateIdx);
		CurrentSize++;
		return true;
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

bool UInventoryComponent::TryRemove(const FGuid& ItemId)
{
	return false;
}

bool UInventoryComponent::TryAdd(const FGuid& ItemId, const int& ArrIdx)
{
	return false;
}

bool UInventoryComponent::TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx)
{
	return false;
}

void UInventoryComponent::TryGetItem(const FGuid& Itemid, FItemData ResultData)
{
	return;
}

void UInventoryComponent::TryGetItemAtIndex(const FGuid& Itemid, const int& ArrIdx, FItemData ResultData)
{
	return;
}

