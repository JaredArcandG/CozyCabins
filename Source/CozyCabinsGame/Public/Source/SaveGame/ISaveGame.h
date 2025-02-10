// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "ISaveGame.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCustomSaveGame : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for savable/loadable objects and information
 * This represents actions to take during or before the save/load of the game, such as updating the structs/variables, NOT SAVING/LOADING THE OBJECT ITSELF
 */
class COZYCABINSGAME_API ICustomSaveGame
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Called when the object is saved
	/// </summary>
	/// <param name="WorldContextObject"></param>
	/// <param name="ObjectToSave"></param>
	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) = 0;

	/// <summary>
	/// Called when the object is loaded
	/// </summary>
	/// <param name="WorldContextObject"></param>
	/// <param name="ObjectToLoad"></param>
	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) = 0;

	/// <summary>
	/// Called for multiple unique objects being saved into the save game
	/// </summary>
	/// <param name="WorldContextObject"></param>
	/// <param name="ObjId"></param>
	/// <param name="ObjectToSave"></param>
	virtual void OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave) = 0;

	/// <summary>
	/// Called to load multiple unique objects if needed
	/// </summary>
	/// <param name="WorldContextObject"></param>
	/// <param name="ObjId"></param>
	/// <param name="ObjectToLoad"></param>
	virtual void OnLoadUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToLoad) = 0;

	/// <summary>
	/// Called to clear a slot if it's being overwritten
	/// Used to avoid old save data being saved with the new save
	/// </summary>
	virtual void ClearOverwrite() = 0;
};
