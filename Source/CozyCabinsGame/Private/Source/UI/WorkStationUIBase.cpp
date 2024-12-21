// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/WorkStationUIBase.h"
#include "Source/Crafting/CraftingData.h"
#include "Source/UI/CraftingRecipeSlot.h"
#include "Source/UI/CraftingIngredientSlot.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Items/ItemData.h"
#include "GameFramework/Character.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


void UWorkStationUIBase::InitializeStation(TObjectPtr<class UDataTable> DataTable, TObjectPtr<ACharacter> SourceCharacter)
{
	if (SourceCharacter)
	{
		if (InventoryComponent = SourceCharacter->FindComponentByClass<UInventoryComponent>()) 
		{
			TArray<FName> DataTableRowNames = DataTable->GetRowNames();

			for (FName CurrentRow : DataTableRowNames)
			{
				FCraftingRecipe* NewRecipe = DataTable->FindRow<FCraftingRecipe>(CurrentRow, "");
				TObjectPtr<UCraftingRecipeSlot> NewRecipeSlot = CreateWidget<UCraftingRecipeSlot>(GetWorld(), RecipeSlotWidget);
				if (NewRecipe && NewRecipeSlot) 
				{
					NewRecipeSlot->InitializeSlot(NewRecipe);
					WrapBox->AddChildToWrapBox(NewRecipeSlot);
				}
			}

			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;

			AddToViewport();
		}
	}
}

void UWorkStationUIBase::ChangeSelection(FCraftingRecipe* ActiveRecipe)
{
	CurrentRecipe = *ActiveRecipe;

	FItemData* CraftingResultData = ActiveRecipe->CraftingResult.ItemReference.DataTable->FindRow<FItemData>(ActiveRecipe->CraftingResult.ItemReference.RowName, "");
	TArray<FCraftingItem> IngredientsData = ActiveRecipe->CraftingIngredients;


	SelectedItemImage->SetBrushFromTexture(CraftingResultData->Icon);
	SelectedItemName->SetText(FText::AsCultureInvariant(CraftingResultData->Name));

	SelectedItemIngredients->ClearChildren();

	for (FCraftingItem Ingredient : IngredientsData) 
	{
		UCraftingIngredientSlot* IngredientSlot = CreateWidget<UCraftingIngredientSlot>(GetWorld(), IngredientSlotWidget);
		SelectedItemIngredients->AddChildToVerticalBox(IngredientSlot);
	}
}

void UWorkStationUIBase::AttemptCraft()
{
	TArray<FCraftingItem> IngredientsData = CurrentRecipe.CraftingIngredients;

	if (InventoryComponent)
	{
		for (FCraftingItem Ingredient : IngredientsData) 
		{
			TArray<int> Indicies = InventoryComponent->GetIndexesWithItem(Ingredient.ItemReference.DataTable->FindRow<FItemData>(Ingredient.ItemReference.RowName, "")->Id);
			Ingredient.ItemReference.RowName;
		}
	}
}
