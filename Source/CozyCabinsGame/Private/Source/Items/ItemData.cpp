// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Items/ItemData.h"

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
	Icon = Other.Icon;
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
