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
	int CurrentGameSlotIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumMaxGameSlots = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveGameSlotNameBase = "CozySaveGame";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCraftingSaveGame> CraftingSaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCraftingSaveGame> CraftingSaveGameClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UDataTable>> CraftingDataTables;

protected:
	
	UPROPERTY()
	TArray<TObjectPtr<class USaveGameSlot>> GameSlots;

public:

	/// Save Game Public Functions ///

	UFUNCTION()
	void OnSaveGame(const int& ChosenSlotIdx);

	UFUNCTION()
	void OnLoadGame(const int& ChosenSlotIdx);

	UFUNCTION()
	void InitializeSaveGameSlots(UWorld* World);

	TArray<TObjectPtr<class USaveGameSlot>> GetGameSlots();

protected:

	// Called on Start of Application
	virtual void Init() override;

};
