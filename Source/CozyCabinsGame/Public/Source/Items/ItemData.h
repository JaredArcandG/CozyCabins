// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * Enum representing an Crop Stage
 */
UENUM(BlueprintType)
enum class ECropStage : uint8
{
	StageOne,
	StageTwo,
	StageThree,
	Completed
};
ENUM_RANGE_BY_FIRST_AND_LAST(ECropStage, ECropStage::StageOne, ECropStage::StageThree);


/**
 * Enum representing an Item Category
 */
UENUM(BlueprintType)
enum class EItemCatgeory : uint8
{
	Plantable
};

/**
 * Item Data for a singular Crop Stage
 */
USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FCropItemData
{
	GENERATED_BODY()

public:

	//Represents the Amount of time in this Stage in Minutes [Example: StageOne = 20.0f aka 20 Ingame Minutes]
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Crop Stage Time"))
	float StageTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Crop Stage Mesh"))
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	FCropItemData()
		: StageTime(0.0f),
		Mesh(nullptr) { }
};

/**
 * Item Data to be stored in the Item Data Table
 */
USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Unique Item Id - Generated Automatically"))
	FGuid Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item Name"))
	FString Name = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Description of Item"))
	FString Description = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Category of Item"))
	TArray<EItemCatgeory> ItemCategories = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item Icon to show in Widgets"))
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item Mesh reference"))
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Whether the item is consumable or not"))
	bool IsConsumable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Whether the item can be dropped or not"))
	bool IsDroppable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Item BP class reference"))
	TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Text = "Data for each Crop Stage"))
	TMap<ECropStage, FCropItemData> CropStageData;

	FItemData() 
		: Id(FGuid::NewGuid()),
		Name(FString()),
		Description(FString()),
		ItemCategories({}),
		Icon(nullptr),
		Mesh (nullptr),
		IsConsumable(false),
		IsDroppable(false),
		ItemClass(nullptr),
		CropStageData(){ }

	FItemData(const FItemData& Other);
	FItemData& operator=(FItemData const& Other);
	bool operator==(FItemData const& Other) const;
	bool operator!=(FItemData const& Other) const;
	bool operator<(FItemData const& Other) const;
};
