// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotHoverPreview.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API UItemSlotHoverPreview : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> ItemSizeBox;

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemDescTextBlock;

public:

	void SetHoverPreviewSlotData(TObjectPtr<UImage> Image, const FText& ItemName, const FText& ItemDescription);
	
};
