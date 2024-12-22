// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/CraftingRecipeSlot.h"
#include "Source/UI/WorkStationUIBase.h"
#include "Source/Items/ItemData.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UCraftingRecipeSlot::NativeConstruct()
{
	ButtonSelect->OnPressed.AddDynamic(this, &UCraftingRecipeSlot::ChangeSlot);
}

void UCraftingRecipeSlot::InitializeSlot(FCraftingRecipe* Recipe, TObjectPtr<UUserWidget> ParentReference)
{
	CraftingRecipe = Recipe;
	ParentWidget = ParentReference;

	if (CraftingRecipe) 
	{
		FItemData* ItemData = CraftingRecipe->CraftingResult.ItemReference.DataTable->FindRow<FItemData>(CraftingRecipe->CraftingResult.ItemReference.RowName, "");
		if (ItemData) 
		{
			if (ItemData->Icon) 
			{
				ItemIcon->SetBrushFromTexture(ItemData->Icon);
			}
		}
	}
}

void UCraftingRecipeSlot::ChangeSlot()
{
	if (ParentWidget) 
	{
		TObjectPtr<UWorkStationUIBase> WorkstationUI = Cast<UWorkStationUIBase>(ParentWidget);

		if (WorkstationUI) 
		{
			WorkstationUI->ChangeSelection(CraftingRecipe);
		}
	}
}
