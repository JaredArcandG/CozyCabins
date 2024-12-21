// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/CraftingRecipeSlot.h"
#include "Source/Items/ItemData.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UCraftingRecipeSlot::InitializeSlot(FCraftingRecipe* Recipe)
{
	FItemData* ItemData = Recipe->CraftingResult.ItemReference.DataTable->FindRow<FItemData>(Recipe->CraftingResult.ItemReference.RowName, "");
	if (ItemData) 
	{
		if (ItemData->Icon) 
		{
			ItemIcon->SetBrushFromTexture(ItemData->Icon);
		}
	}
}
