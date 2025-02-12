// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotHoverPreview.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FHoverPreviewData
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UImage> Image;

	UPROPERTY(BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly)
	FText ItemDescription;

};

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

	void SetHoverPreviewSlotData(TObjectPtr<class UImage> Image, const FText& ItemName, const FText& ItemDescription);
	
};
