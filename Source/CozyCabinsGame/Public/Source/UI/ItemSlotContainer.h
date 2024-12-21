// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <UObject/NoExportTypes.h>
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

protected:

	TArray<TObjectPtr<class UItemSlot>> ItemSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Settings")
	int TotalSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Settings")
	TSubclassOf<class UItemSlot> ItemSlotClass;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryCompRef;

public:

	void Setup(class UInventoryComponent& Inventory);

};
