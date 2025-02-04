// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"


UCLASS()
class COZYCABINSGAME_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/// Save Game Properties ///

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SaveGameSlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCraftingSaveGame> CraftingSaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCraftingSaveGame> CraftingSaveGameClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CraftingSaveGame_SlotName = "CraftingSaveGame";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UDataTable>> CraftingDataTables;

	/// Save Game Public Functions ///

	// Save the Crafting SaveGame
	UFUNCTION()
	void SaveCraftingSaveGame();

private:

	/// Save Game PrivateFunctions ///

	// Gets all SaveGames
	UFUNCTION()
	void InitializeSaveGames();


protected:

	// Called on Start of Application
	virtual void Init() override;

};
