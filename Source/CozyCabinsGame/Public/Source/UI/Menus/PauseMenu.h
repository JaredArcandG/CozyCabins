// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> OptionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SaveButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> LoadButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> QuitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USaveLoadContainer> SaveLoadContainerClass;

public:

	void TogglePauseMenu();

protected:

	UPROPERTY()
	TObjectPtr<class USaveLoadContainer> SaveLoadMenu;

protected:

	virtual void NativeConstruct() override;

	void ShowPauseMenu();

	void HidePauseMenu();

	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnSaveClicked();

	UFUNCTION()
	void OnLoadClicked();

	UFUNCTION()
	void OnQuitClicked();
};
