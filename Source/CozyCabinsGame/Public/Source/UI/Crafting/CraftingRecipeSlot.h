// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Source/Crafting/CraftingData.h"
#include "CraftingRecipeSlot.generated.h"


UCLASS()
class COZYCABINSGAME_API UCraftingRecipeSlot : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ButtonSelect;

	FCraftingRecipe* CraftingRecipe;

	UPROPERTY()
	TObjectPtr<UUserWidget> ParentWidget;

	// Called through Button press, attempting to change the WorkStationUIBase active Recipe
	UFUNCTION()
	void ChangeSlot();

public:

	// Called when Workstation is Interacted with at first
	void InitializeSlot(FCraftingRecipe* Recipe, TObjectPtr<UUserWidget> ParentReference);
};
