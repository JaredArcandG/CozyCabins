// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CraftingData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FCraftingItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Quantity of Item"))
	int Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "DataTable reference of Item"))
	FDataTableRowHandle ItemReference;

};

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "All Items needed to create the Crafting Result"))
	TArray<FCraftingItem> CraftingIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Crafting Result"))
	FCraftingItem CraftingResult;

};