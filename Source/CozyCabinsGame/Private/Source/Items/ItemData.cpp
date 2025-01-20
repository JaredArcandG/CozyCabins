// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Items/ItemData.h"
#include "Source/Items/Item.h"

FItemData::FItemData(const FItemData& Other)
{
	*this = Other;
}

FItemData& FItemData::operator=(FItemData const& Other)
{
	if (this == &Other)
	{
		return *this;
	}

	Id = Other.Id;
	Name = Other.Name;
	Description = Other.Description;
	ItemCategories = Other.ItemCategories;
	Icon = Other.Icon;
	Mesh = Other.Mesh;
	IsConsumable = Other.IsConsumable;
	IsDroppable = Other.IsDroppable;
	ItemClass = Other.ItemClass;
	CropStageData = Other.CropStageData;
	IsRespawnableOnDrop = Other.IsRespawnableOnDrop;
	RespawnTimeInGameTime = Other.RespawnTimeInGameTime;
	return *this;
}

bool FItemData::operator==(FItemData const& Other) const
{
	return Other.Id == this->Id;
}

bool FItemData::operator!=(FItemData const& Other) const
{
	return Other.Id != this->Id;
}

bool FItemData::operator<(FItemData const& Other) const
{
	return this->Id < Other.Id;
}
