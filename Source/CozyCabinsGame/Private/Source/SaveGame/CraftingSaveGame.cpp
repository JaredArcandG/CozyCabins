// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/CraftingSaveGame.h"

TArray<FName> UCraftingSaveGame::ReturnUnlockedRecipes(UDataTable* TableToScan)
{
	TArray<FName> UnlockedRecipeRows;

	if (UnlockedRecipesMap.Contains(TableToScan)) 
	{
		FCraftingRecipeUnlock* UnlockedRecipesStruct = UnlockedRecipesMap.Find(TableToScan);

		for (auto& Elem : UnlockedRecipesStruct->UnlockedRecipesMap) 
		{
			if (Elem.Value) 
			{
				UnlockedRecipeRows.Add(Elem.Key);
			}
		}
	}
	else 
	{
		FCraftingRecipeUnlock NewStruct;
		UnlockedRecipesMap.Add(TableToScan, NewStruct);
	}

	return UnlockedRecipeRows;
}


void UCraftingSaveGame::UnlockRecipe(UDataTable* TableToScan, FName RecipeRow)
{
	if (!UnlockedRecipesMap.Contains(TableToScan))
	{
		FCraftingRecipeUnlock NewStruct;
		UnlockedRecipesMap.Add(TableToScan, NewStruct);
	}

	FCraftingRecipeUnlock* UnlockedRecipesStruct = UnlockedRecipesMap.Find(TableToScan);
	UnlockedRecipesStruct->UnlockedRecipesMap.Add(RecipeRow, true);

}

void UCraftingSaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
}

void UCraftingSaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad)
{
}
