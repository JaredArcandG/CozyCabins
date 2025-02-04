// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Source/Crafting/CraftingData.h"
#include "CraftingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UCraftingSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Text = "Map of Recipes which are unlocked"))
	TMap<TObjectPtr<class UDataTable>, FCraftingRecipeUnlock> UnlockedRecipesMap;

	UFUNCTION(BlueprintCallable)
	TArray<FName> ReturnUnlockedRecipes(UDataTable* TableToScan);

	UFUNCTION(BlueprintCallable)
	void UnlockRecipe(UDataTable* TableToScan, FName RecipeRow);
};
