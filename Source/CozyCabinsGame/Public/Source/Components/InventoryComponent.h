// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Source/Items/ItemData.h>
#include "InventoryComponent.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Settings")
	TSubclassOf<class UItemSlotContainer> ItemSlotContainerClass;

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

	UPROPERTY()
	TObjectPtr<class UItemSlotContainer> ItemSlotContainer;

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

	void ToggleInventory();

	TArray<int> GetIndexesWithItem(const FGuid& TargetGuid);

protected:

	bool TryGetQuantityAtIndex(const int& Idx, int& Quantity);

};
