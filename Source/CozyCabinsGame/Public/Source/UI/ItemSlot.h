// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Source/Items/ItemData.h>
#include "Source/UI/DraggableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemSlot.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API UItemSlot : public UDraggableWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ItemButton;

	UPROPERTY()
	TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UItemSlotDragPreview> DragPreviewClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UItemSlotHoverPreview> HoverPreviewClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D HoverPreviewOffset;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryCompRef;

	FGuid ItemId;

	int ItemQty;

	FText ItemDescription;

	// Reference to where in the inventory the slot is located
	int InventoryIdx;

	bool bIsOccupied;

	int PreviewQtyToTransfer;

	// Refers to whether the item can be dropped in the world
	// Shouldn't be droppable for key items
	bool bIsDroppable;

	UPROPERTY()
	TObjectPtr<class UItemSlotDragPreview> DragPreviewWidget;

	UPROPERTY()
	TObjectPtr<class UItemSlotHoverPreview> HoverPreviewWidget;

	UPROPERTY()
	TObjectPtr<class ACustomPlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<class UGlobalUIManager> GlobalUIManager;

	UPROPERTY()
	TObjectPtr<class UInteractableComponent> InteractableComp;

public:

	void SetSlotData(const FItemData& ItemData, const int& Amount, const int& IdxInInventory, class UInventoryComponent& InventoryComp);

	void SetEmptySlot(const int& IdxInInventory, UInventoryComponent& InventoryComp);

	void HandleOnClose();

protected:
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void OnConsumeItem();

	virtual FReply NativeOnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void ShowPreviewWidgetPreDrag();

	void UpdatePreviewWidgetPreDrag();

	void RemovePreviewWidgetPreDrag();

	void DestroyHoverPreviewWidget();

	void HideHoverPreviewWidget();

};
