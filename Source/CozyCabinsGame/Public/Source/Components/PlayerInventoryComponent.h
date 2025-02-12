// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Items/Item.h"
#include "Source/Notifications/ItemNotification.h"
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

	UPlayerInventoryComponent();

	// Event for Inventory item change
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInventoryChange, UBaseNotification*, ItemNotification);

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerInventoryChange OnPlayerInventoryChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ItemWidgetNotificationClass;

protected:

	bool bDisableNotification;

public:

	virtual bool TryAddAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity) override;

	virtual bool TryRemoveAtIndex(const FGuid& ItemId, const int& ArrIdx, const int& Quantity) override;

	virtual bool TryTransferSlots(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& TargetSlotIdx);

	virtual bool TryTransferSlotsWithQuantity(UInventoryComponent* TargetInventory, const int& SourceSlotIdx, const int& SourceQtyToTransfer, const int& TargetSlotIdx);

protected:

	void SendItemNotification(const FGuid& ItemId, const EItemAction& Action, const int& Quantity);
	
};
