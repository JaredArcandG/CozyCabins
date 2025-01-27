// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Storage/Chest.h"
#include "Source/Components/InventoryComponent.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/Player/PlayerCharacter.h>
#include <Source/UI/ChestUI.h>
#include "Components/StaticMeshComponent.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));


}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();

	CHECK(InventoryComp);
	CHECK(ChestUIClass);
	
	PlayerController = GetWorld()->GetFirstPlayerController();
	CHECK(PlayerController);

	Player = Cast<APlayerCharacter>(PlayerController->GetPawn());
	CHECK(Player);

	if (auto pPlayerInventoryComp = Player->GetComponentByClass<UInventoryComponent>())
	{
		// Create the widget, set it up with the two required inventory components
		ChestUIWidget = CreateWidget<UChestUI>(GetWorld(), ChestUIClass);
		CHECK(ChestUIWidget);

		ChestUIWidget->AddToViewport(3);
		ChestUIWidget->SetVisibility(ESlateVisibility::Hidden);
		ChestUIWidget->Setup(*pPlayerInventoryComp, *InventoryComp);
	}
	
}

/// <summary>
/// Chest action on interact 
/// Opens the chest inventory
/// </summary>
/// <param name="World"></param>
/// <param name="SourceCharacter"></param>
void AChest::OnInteract_Implementation(UWorld* World, ACharacter* SourceCharacter)
{
	if (bIsChestInventoryOpen)
	{
		CloseChestUIWidget();
	}
	else
	{
		OpenChestUIWidget();
	}

	bIsChestInventoryOpen = !bIsChestInventoryOpen;
}

/// <summary>
/// Open the chest UI
/// </summary>
void AChest::OpenChestUIWidget()
{
	CHECK(ChestUIWidget);
	CHECK(PlayerController);
	CHECK(Player);

	// Disable player movement while interacting with chest
	Player->AllowPlayerMove(false);
	Player->AllowPlayerLook(false);

	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;

	ChestUIWidget->SetVisibility(ESlateVisibility::Visible);
	ChestUIWidget->UpdateUI();
}

/// <summary>
/// Close the chest UI
/// </summary>
void AChest::CloseChestUIWidget()
{
	CHECK(ChestUIWidget);
	CHECK(PlayerController);
	CHECK(Player);

	// Enable player movement again
	Player->AllowPlayerMove(true);
	Player->AllowPlayerLook(true);

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	ChestUIWidget->SetVisibility(ESlateVisibility::Hidden);
}

