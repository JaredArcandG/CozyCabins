// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotDragPreview.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemSlotDragPreview : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventoryIdx;

public:

	void SetPreviewSlotData(const FGuid& Id, const int& Amount, const int& IdxInIvnentory, class UImage& Image);
	
};
