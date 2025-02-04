// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CraftingLibrary.generated.h"

class UDataTable;

UCLASS()
class COZYCABINSGAME_API UCraftingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static TArray<FName> ReturnUnlockedRecipes(UDataTable* TableToScan, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static void UnlockRecipe(UDataTable* TableToScan, const UObject* WorldContext, FName RecipeRow);
};
