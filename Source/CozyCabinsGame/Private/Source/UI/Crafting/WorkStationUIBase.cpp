// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Crafting/WorkStationUIBase.h"
#include "Source/UI/Crafting/CraftingRecipeSlot.h"
#include "Source/UI/Crafting/CraftingIngredientSlot.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Items/ItemData.h"
#include "Source/Libraries/CraftingLibrary.h"
#include "GameFramework/Character.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWorkStationUIBase::NativeConstruct()
{

}

void UWorkStationUIBase::InitializeStation(TObjectPtr<class UDataTable> DataTable, TObjectPtr<ACharacter> SourceCharacter, FCraftingItem FuelItemValue, bool RequireFuelItemValue)
{
	if (SourceCharacter)
	{
		if (InventoryComponent = SourceCharacter->FindComponentByClass<UInventoryComponent>()) 
		{
			TArray<FName> DataTableRowNames = DataTable->GetRowNames();
			WrapBox->ClearChildren();

			RequireFuelItem = RequireFuelItemValue;
			FuelItem = FuelItemValue;

			// Iterrate and Add all Default Unlocked Recipes
			for (FName CurrentRow : DataTableRowNames)
			{
				FCraftingRecipe* NewRecipe = DataTable->FindRow<FCraftingRecipe>(CurrentRow, "");
				if (NewRecipe->UnlockedByDefault) 
				{
					TObjectPtr<UCraftingRecipeSlot> NewRecipeSlot = CreateWidget<UCraftingRecipeSlot>(GetWorld(), RecipeSlotWidget);
					if (NewRecipe && NewRecipeSlot) 
					{
						NewRecipeSlot->InitializeSlot(NewRecipe, this);
						WrapBox->AddChildToWrapBox(NewRecipeSlot);
					}
				}
			}

			// Get All Unlocked Recipes from this DataTable
			TArray<FName> UnlockedRecipes = UCraftingLibrary::ReturnUnlockedRecipes(DataTable, InventoryComponent->GetOwner());

			// Iterrate and Add all Unlocked Recipes
			for (FName CurrentRow : UnlockedRecipes)
			{
				FCraftingRecipe* NewUnlockedRecipe = DataTable->FindRow<FCraftingRecipe>(CurrentRow, "");

				TObjectPtr<UCraftingRecipeSlot> NewUnlockedRecipeSlot = CreateWidget<UCraftingRecipeSlot>(GetWorld(), RecipeSlotWidget);
				if (NewUnlockedRecipe && NewUnlockedRecipeSlot)
				{
					NewUnlockedRecipeSlot->InitializeSlot(NewUnlockedRecipe, this);
					WrapBox->AddChildToWrapBox(NewUnlockedRecipeSlot);
				}
			}

			APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;

			SetCraftingMultiplier(1);

			AddToViewport();
		}
	}
}

void UWorkStationUIBase::ChangeSelection(FCraftingRecipe* ActiveRecipe)
{
	CurrentRecipe = ActiveRecipe;
	if (CurrentRecipe) 
	{
		VBOX_CraftingRecipe->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FItemData* CraftingResultData = ActiveRecipe->CraftingResult.ItemReference.DataTable->FindRow<FItemData>(ActiveRecipe->CraftingResult.ItemReference.RowName, "");
		TArray<FCraftingItem> IngredientsData = ActiveRecipe->CraftingIngredients;


		SelectedItemImage->SetBrushFromTexture(CraftingResultData->Icon);
		SelectedItemName->SetText(FText::AsCultureInvariant(CraftingResultData->Name));

		SelectedItemIngredients->ClearChildren();
		SelectedItemFuelIngredients->ClearChildren();

		for (FCraftingItem Ingredient : IngredientsData) 
		{
			UCraftingIngredientSlot* IngredientSlot = CreateWidget<UCraftingIngredientSlot>(GetWorld(), IngredientSlotWidget);
			SelectedItemIngredients->AddChildToVerticalBox(IngredientSlot);
			IngredientSlot->SetupSlot(Ingredient, InventoryComponent, CraftingMultiplier);
		}

		// Check Fuel Ingredients
		if (RequireFuelItem) 
		{
			UCraftingIngredientSlot* IngredientFuelSlot = CreateWidget<UCraftingIngredientSlot>(GetWorld(), IngredientSlotWidget);
			SelectedItemFuelIngredients->AddChildToVerticalBox(IngredientFuelSlot);
			IngredientFuelSlot->SetupSlot(FuelItem, InventoryComponent, CraftingMultiplier);

			SelectedItemFuelIngredients->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			FuelRequirementText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else 
		{
			SelectedItemFuelIngredients->SetVisibility(ESlateVisibility::Collapsed);
			FuelRequirementText->SetVisibility(ESlateVisibility::Collapsed);
		}

		SetCraftingMultiplier(1);
	}
	else 
	{
		VBOX_CraftingRecipe->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWorkStationUIBase::IncreaseCraftingMultiplier()
{
	ChangeCraftingMultiplier(1);
}

void UWorkStationUIBase::DecreaseCraftingMultiplier()
{
	ChangeCraftingMultiplier(-1);
}

void UWorkStationUIBase::ChangeCraftingMultiplier(int Value)
{
	int tempQuantity = CraftingMultiplier + Value;
	CraftingMultiplier = FMath::Clamp(tempQuantity, 1, 99);

	UpdateCraftingResultQuantity();
	UpdateIngredients();
}

void UWorkStationUIBase::SetCraftingMultiplier(int Value)
{
	CraftingMultiplier = FMath::Clamp(Value, 1, 99);

	UpdateCraftingResultQuantity();
	UpdateIngredients();
}

void UWorkStationUIBase::UpdateCraftingResultQuantity()
{
	if (CurrentRecipe) 
	{	
		FString CraftingMultiplierText = "Craft Item x" + FString::FromInt(CurrentRecipe->CraftingResult.Quantity * CraftingMultiplier);
		TXT_CraftingResultQuantity->SetText(FText::AsCultureInvariant(CraftingMultiplierText));
	}
}


bool UWorkStationUIBase::AttemptCraft()
{
	TArray<FCraftingItem> IngredientsData = CurrentRecipe->CraftingIngredients;

	if (RequireFuelItem) 
	{
		IngredientsData.Add(FuelItem);
	}

	if (InventoryComponent)
	{
		// Check if Quantity to craft matches
		if (CheckQuantity(IngredientsData)) 
		{
			// Attempt to remove all Ingrdients
			if (AttemptRemove(IngredientsData)) 
			{
				//Attempt to add Crafting result
				if (AttemptAdd(CurrentRecipe->CraftingResult)) 
				{
					UpdateIngredients();
					return true;
				}
			}		
		}
	}

	return false;
}



bool UWorkStationUIBase::CheckQuantity(TArray<struct FCraftingItem> IngredientsData)
{
	for (FCraftingItem Ingredient : IngredientsData)
	{
		int tempQuantity = 0;
		FItemData* itemData = Ingredient.ItemReference.DataTable->FindRow<FItemData>(Ingredient.ItemReference.RowName, "");
		FItemData tempData;

		if (InventoryComponent->TryGetItem(itemData->Id, tempData, tempQuantity))
		{
			if (tempQuantity < Ingredient.Quantity * CraftingMultiplier)
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	return true;
}

bool UWorkStationUIBase::AttemptRemove(TArray<struct FCraftingItem> IngredientsData)
{
	for (FCraftingItem Ingredient : IngredientsData)
	{
		FItemData* itemData = Ingredient.ItemReference.DataTable->FindRow<FItemData>(Ingredient.ItemReference.RowName, "");

		if (!InventoryComponent->TryRemove(itemData->Id, Ingredient.Quantity * CraftingMultiplier))
		{
			return false;
		}
	}

	return true;
}

bool UWorkStationUIBase::AttemptAdd(FCraftingItem ResultData)
{
	FItemData* itemData = ResultData.ItemReference.DataTable->FindRow<FItemData>(ResultData.ItemReference.RowName, "");

	if (!InventoryComponent->TryAdd(itemData->Id, ResultData.Quantity * CraftingMultiplier))
	{
		return false;
	}

	return true;
}

void UWorkStationUIBase::UpdateIngredients()
{
	TArray<UWidget*> Childs = SelectedItemIngredients->GetAllChildren();
	if (!Childs.IsEmpty()) 
	{
		for (UWidget* Ingredient : Childs)
		{
			if (TObjectPtr<UCraftingIngredientSlot> IngredientSlot = Cast<UCraftingIngredientSlot>(Ingredient)) 
			{
				IngredientSlot->UpdateSlot(InventoryComponent, CraftingMultiplier);
			}
		}
	}

	TArray<UWidget*> FuelChilds = SelectedItemFuelIngredients->GetAllChildren();
	if (!FuelChilds.IsEmpty()) 
	{
		for (UWidget* FuelIngredient : FuelChilds)
		{
			if (TObjectPtr<UCraftingIngredientSlot> FuelIngredientSlot = Cast<UCraftingIngredientSlot>(FuelIngredient)) 
			{
				FuelIngredientSlot->UpdateSlot(InventoryComponent, CraftingMultiplier);
			}
		}
	}

}
