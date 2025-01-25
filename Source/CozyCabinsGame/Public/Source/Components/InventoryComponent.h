// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Source/Items/ItemData.h>
#include "InventoryComponent.generated.h"

#define EMPTY_GUID FGuid()
#define EMPTY_SLOT_VALUE -1

/// <summary>
/// Class to handle logic related to the Inventory
/// Can be attached to multiple characters
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COZYCABINSGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChange);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnInventoryChange OnInventoryChange;

	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Settings")
	int MaxInventorySize = 36;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Settings")
	int MaxItemStackSize = 99;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Settings")
	bool bCanUseInventory = true;

protected:

	// Represents an item stack with the arr idx as the slot number, and value as the quantity of the item
	TArray<int> ItemQuantityArr;

	// Array for item id lookup with the arr idx as the slot number
	TArray<FGuid> ItemIdArr;

	// Keep track of the current array size
	int CurrentSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UDataTable> DataTable;

	bool bIsInventoryOpen;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	int FindEmptyIdxInItemArr();

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to add 'quantity' number of items to the inventory. Returns true if successful, false otherwise"))
	bool TryAdd(const FGuid& ItemId, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to remove the 'quantity' number of items from the inventory. Returns true if successful, false otherwise"))
	bool TryRemove(const FGuid & ItemId, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to add 'quantity' number of items to the inventory for a specific index. Returns true if successful, false otherwise"))
	bool TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to remove the item to the inventory for a specific index with a specific quantity for that index only. Returns true if successful, false otherwise"))
	bool TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to get an item from the item table if it exists in the inventory. ResultData is null if item not found or invalid. Index agnostic, quantity is the total over all indexes."))
	bool TryGetItem(const FGuid& ItemId, FItemData& ResultData, int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to get an item at a specific index in the item table if it exists in the inventory. ResultData is null if item not found or invalid, Quantity is -1 if invalid."))
	bool TryGetItemAtIndex(int ItemArrIdx, FItemData& ResultData, int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Changes the size of the inventory. True if resize was successful, false otherwise. Note: Can only increase the size."))
	bool Resize(const int& NewMaxSize);

	UFUNCTION(BlueprintCallable, meta = (Text = "Swaps the slot between the same or different inventories. Useful for drop functionality calls"))
	bool TryTransferSlots(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& TargetSlotIdx);

	UFUNCTION(BlueprintCallable, meta = (Text = "Swaps the slot between the same or different inventories for specific quantities. Useful for drop functionality calls"))
	bool TryTransferSlotsWithQuantity(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& SourceQtyToTransfer, const int& TargetSlotIdx);

	UFUNCTION(BlueprintCallable, meta = (Text = "Checks if it is possible to remove the item to the inventory for a specific index with a specific quantity for that index only. Returns true if successful, false otherwise"))
	bool CanRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to add 'quantity' number of items to the inventory for a specific index. Returns true if successful, false otherwise"))
	bool CanAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Gets all indexes that have the specified item in the inventory"))
	TArray<int> GetIndexesWithItem(const FGuid& TargetGuid);

	UFUNCTION(BlueprintCallable, meta = (Text = "Gets all indexes that have the specified item in the inventory"))
	bool TryGetFirstItemWithId(const FGuid& InTargetGuid, FItemData& OutResultData, int& OutQuantity, int& OutIndex);

	UFUNCTION(Blueprintcallable, meta = (Text = "Get all item indexes where the item meets the specified filter params"))
	TArray<int> GetIndexesWithItemFilter(const struct FItemSearchFilterParams& FilterParams);

protected:

	bool TryGetQuantityAtIndex(const int& Idx, int& Quantity);

};
