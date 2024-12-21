// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkStationUIBase.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UWorkStationUIBase : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SelectedItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> SelectedItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> SelectedItemIngredients;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SelectedItemCraft;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<struct FCraftingRecipe> CurrentRecipe;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCraftingRecipeSlot> RecipeSlotWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCraftingIngredientSlot> IngredientSlotWidget;

	void InitializeStation(TObjectPtr<class UDataTable> DataTable, TObjectPtr<ACharacter> SourceCharacter);

	void ChangeSelection(struct FCraftingRecipe* ActiveRecipe);

	void AttemptCraft();

};
