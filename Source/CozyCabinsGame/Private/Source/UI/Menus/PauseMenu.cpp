// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Menus/PauseMenu.h"
#include "Components/Button.h"
#include <Source/Utils/CheckUtils.h>
#include "Source/UI/SaveLoad/SaveLoadContainer.h"
#include <Kismet/GameplayStatics.h>

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	CHECK(SaveLoadContainerClass);
	SaveLoadMenu = CreateWidget<USaveLoadContainer>(this, SaveLoadContainerClass);
	SaveLoadMenu->SetVisibility(ESlateVisibility::Hidden);

	SaveLoadMenu->AddToViewport(5);

	CHECK(ResumeButton);
	CHECK(OptionsButton);
	CHECK(SaveButton);
	CHECK(LoadButton);
	CHECK(QuitButton);

	ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnResumeClicked);
	OptionsButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnOptionsClicked);
	SaveButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnSaveClicked);
	LoadButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnLoadClicked);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnQuitClicked);
}

void UPauseMenu::TogglePauseMenu()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		HidePauseMenu();
	}
	else
	{
		ShowPauseMenu();
	}
}

void UPauseMenu::ShowPauseMenu()
{
	FInputModeUIOnly inputMode;

	SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void UPauseMenu::HidePauseMenu()
{
	FInputModeGameOnly inputMode;

	SetVisibility(ESlateVisibility::Hidden);

	GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void UPauseMenu::OnResumeClicked()
{
	HidePauseMenu();
}

void UPauseMenu::OnOptionsClicked()
{
	// TODO: Implement Options Widget (Game Audio, Graphics, etc.)
}

void UPauseMenu::OnSaveClicked()
{
	CHECK(SaveLoadMenu);

	SaveLoadMenu->UpdateSlots();
	SaveLoadMenu->SetSlotMode(ESaveLoadMode::Save);
	SaveLoadMenu->SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenu::OnLoadClicked()
{
	CHECK(SaveLoadMenu);

	SaveLoadMenu->UpdateSlots();
	SaveLoadMenu->SetSlotMode(ESaveLoadMode::Load);
	SaveLoadMenu->SetVisibility(ESlateVisibility::Visible);
}

void UPauseMenu::OnQuitClicked()
{
	TObjectPtr<APlayerController> pPC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CHECK(pPC);

	if (pPC)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), pPC, EQuitPreference::Quit, true);
	}
}
