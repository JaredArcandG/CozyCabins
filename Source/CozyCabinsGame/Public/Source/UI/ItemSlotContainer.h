// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <UObject/NoExportTypes.h>
#include "Source/UI/ItemSlotHoverPreview.h"
#include "ItemSlotContainer.generated.h"


/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemSlotContainer : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> GridBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UItemSlotHoverPreview> HoverPreviewWidget;

protected:

	UPROPERTY()
	TArray<TObjectPtr<class UItemSlot>> ItemSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Settings")
	int TotalSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Settings")
	TSubclassOf<class UItemSlot> ItemSlotClass;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryCompRef;

public:

	void Setup(class UInventoryComponent& Inventory);

	UFUNCTION()
	void UpdateAllSlots();

	void HandleOnCloseSlotContainer();

protected:

	void CreateNewItemSlotAddToGrid(const int& ItemIdx);

	UFUNCTION()
	void ShowHoverPreviewWidget(const FHoverPreviewData HoverPreviewData);

	UFUNCTION()
	void HideHoverPreviewWidget();

};
