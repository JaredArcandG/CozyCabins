// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Items/Item.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * Inventory Component specifically for the player
 * Allows broadcasting for events on select functions
 */
UCLASS()
class COZYCABINSGAME_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:

	// Event for Inventory item change
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInventoryChange, FItemNotification, ItemNotification);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerInventoryChange OnPlayerInventoryChange;

public:

	virtual bool TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity) override;

	virtual bool TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity) override;

protected:

	void SendItemNotification(const FGuid& ItemId, const EItemAction& Action, const int& Quantity);
	
};
