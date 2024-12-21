// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * Item Data to be stored in the Item Data Table
 */
USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Unique Item Id - Generated Automatically"))
	FGuid Id = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item Name"))
	FString Name = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Description of Item"))
	FString Description = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item Icon to show in Widgets"))
	TObjectPtr<UTexture2D> Icon = nullptr;

	FItemData() 
		: Id(FGuid::NewGuid()),
		Name(FString()),
		Description(FString()),
		Icon(nullptr) { }

	FItemData(const FItemData& Other);
	FItemData& operator=(FItemData const& Other);
	bool operator==(FItemData const& Other) const;
	bool operator!=(FItemData const& Other) const;
	bool operator<(FItemData const& Other) const;
};
