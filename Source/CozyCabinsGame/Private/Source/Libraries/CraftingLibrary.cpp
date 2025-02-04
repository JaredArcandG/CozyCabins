// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Libraries/CraftingLibrary.h"
#include "Source/SaveGame/CraftingSaveGame.h"
#include "Source/GameInstance/CustomGameInstance.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

TArray<FName> UCraftingLibrary::ReturnUnlockedRecipes(UDataTable* TableToScan, const UObject* WorldContext)
{
	TArray<FName> UnlockedRecipesRows;

	if (WorldContext) 
	{
		TObjectPtr<UCustomGameInstance> CustomGameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
		
		if (CustomGameInstance)
		{
			if (CustomGameInstance->CraftingSaveGame) 
			{
				UnlockedRecipesRows = CustomGameInstance->CraftingSaveGame->ReturnUnlockedRecipes(TableToScan);
			}
		}
	}

	return UnlockedRecipesRows;
}

void UCraftingLibrary::UnlockRecipe(UDataTable* TableToScan, const UObject* WorldContext, FName RecipeRow)
{
	if (WorldContext)
	{
		TObjectPtr<UCustomGameInstance> CustomGameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));

		if (CustomGameInstance)
		{
			if (CustomGameInstance->CraftingSaveGame)
			{
				CustomGameInstance->CraftingSaveGame->UnlockRecipe(TableToScan, RecipeRow);
				CustomGameInstance->SaveCraftingSaveGame();
			}
		}
	}
}
