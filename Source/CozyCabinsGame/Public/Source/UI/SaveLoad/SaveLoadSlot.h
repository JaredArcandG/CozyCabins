// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoadSlot.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API USaveLoadSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int, Index);

	FOnSlotClicked OnSlotClicked;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SlotButton;

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> GameImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SaveTimeText;

	int SlotIdx;

public:

	void Setup();

	virtual void NativeConstruct() override;

	void SetSlotData(class UTexture2D& InImage, const FText& InSaveTimeText, const int& InSlotIdx);

	void ClearSlot();

	UFUNCTION(BlueprintCallable)
	void ExecuteSlotClicked();
	
	
};
