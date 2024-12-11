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

	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Settings")
	int MaxInventorySize = 36;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Settings")
	int MaxItemStackSize = 99;

protected:

	// Represents an item stack with the arr idx as the slot number, and the quantity of the item
	TArray<int> ItemArr;

	// Map for quick lookup. Key is the item id, value is the index in the array where the item was stored
	TMap<FGuid, TArray<int>> ItemCacheMap;

	// Keep track of the current array size
	int CurrentSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UDataTable> DataTable;

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

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to add the item to the inventory for a specific index. Returns true if successful, false otherwise"))
	bool TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Tries to get an item from the item table if it exists in the inventory. ResultData is null if item not found or invalid."))
	bool TryGetItem(const FGuid& ItemId, FItemData ResultData);

};
