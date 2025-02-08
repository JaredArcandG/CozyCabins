// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "Source/Crafting/CraftingData.h"
#include "CraftingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UCraftingSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Text = "Map of Recipes which are unlocked"))
	TMap<TObjectPtr<class UDataTable>, FCraftingRecipeUnlock> UnlockedRecipesMap;

	UFUNCTION(BlueprintCallable)
	TArray<FName> ReturnUnlockedRecipes(UDataTable* TableToScan);

	UFUNCTION(BlueprintCallable)
	void UnlockRecipe(UDataTable* TableToScan, FName RecipeRow);

public:

	virtual void OnSave(UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad) override;
};
