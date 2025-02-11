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


void UCraftingSaveGame::UnlockRecipe(UDataTable* TableToScan, const FName& RecipeRow)
{
	if (!UnlockedRecipesMap.Contains(TableToScan))
	{
		FCraftingRecipeUnlock NewStruct;
		UnlockedRecipesMap.Add(TableToScan, NewStruct);
	}

	FCraftingRecipeUnlock* UnlockedRecipesStruct = UnlockedRecipesMap.Find(TableToScan);
	UnlockedRecipesStruct->UnlockedRecipesMap.Add(RecipeRow, true);

}

void UCraftingSaveGame::OnSave(const UObject& WorldContextObject, UObject& ObjectToSave)
{
	for (auto pDataTable : DataTables)
	{
		if (pDataTable)
		{
			TArray<FName> recipesArr = ReturnUnlockedRecipes(pDataTable);

			for (const FName& recipeName : recipesArr)
			{
				UnlockRecipe(pDataTable, recipeName);
			}
		}
	}
}

void UCraftingSaveGame::OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad)
{
	// No actions needed as we are using the save game values directly
}
