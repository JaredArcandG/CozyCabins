// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/Crafting/CraftingData.h"
#include "Blueprint/UserWidget.h"
#include "WorkStationUIBase.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UWorkStationUIBase : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

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

	FCraftingRecipe* CurrentRecipe;

	// Functions used during the crafting process
	bool CheckQuantity(TArray<struct FCraftingItem> IngredientsData);
	bool AttemptRemove(TArray<struct FCraftingItem> IngredientsData);
	bool AttemptAdd(struct FCraftingItem ResultData);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCraftingRecipeSlot> RecipeSlotWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UCraftingIngredientSlot> IngredientSlotWidget;

	// Called when Interacting with the Station
	void InitializeStation(TObjectPtr<class UDataTable> DataTable, TObjectPtr<ACharacter> SourceCharacter);

	// Called when a new Recipe is being clicked
	void ChangeSelection(FCraftingRecipe* ActiveRecipe);

	// Called from Blueprint when attempting to Craft the selected Recipe
	UFUNCTION(BlueprintCallable)
	bool AttemptCraft();

};
