// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Player/PlayerCharacter.h"
#include "Source/Utils/CheckUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include <EnhancedInputSubsystems.h>
#include <Source/Player/Input/InputConfigData.h>
#include <EnhancedInputComponent.h>
#include "Source/Components/InteractableComponent.h"
#include "Source/Components/PlayerInventoryComponent.h"
#include "Source/Components/StatsComponent.h"
#include "Source/UI/Items/ItemSlotContainer.h"
#include "Source/GameInstance/CustomGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "Source/UI/Menus/PauseMenu.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bCanMove = true;
	bCanLook = true;
	bCanInteract = true;
	bIsInventoryOpen = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create camera boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CHECK(SpringArm);

	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f; // The camera follows at this distance from the character 
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CHECK(FollowCamera);

	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Rotation should affect the Camera only, not the Controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character turns in the direction the controller is moving, but only on the Y axis
	TObjectPtr<UCharacterMovementComponent> pMovementComponent = GetCharacterMovement();

	CHECK(pMovementComponent);

	pMovementComponent->bOrientRotationToMovement = true;
	pMovementComponent->RotationRate = FRotator(0.f, 540.f, 0.f);

	InteractionComp = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractionComponent"));

	InventoryComp = CreateDefaultSubobject<UPlayerInventoryComponent>(TEXT("InventoryComponent"));

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));

}

/// <summary>
/// Toggles whether player can move or not 
/// Can use for cutscenes, pause menus, etc.
/// </summary>
/// <param name="bCanMoveState"></param>
void APlayerCharacter::AllowPlayerMove(const bool& bCanMoveState)
{
	bCanMove = bCanMoveState;
}

/// <summary>
/// Toggles whether player can look or not
/// Can use for cutscenes, pause menus, etc.
/// </summary>
/// <param name="bCanLookState"></param>
void APlayerCharacter::AllowPlayerLook(const bool& bCanLookState)
{
	bCanLook = bCanLookState;
}

/// <summary>
/// Toggles whether player can interact with environment or not
/// </summary>
/// <param name="bCanLookState"></param>
void APlayerCharacter::AllowPlayerInteract(const bool& bCanInteractState)
{
	bCanInteract = bCanInteractState;
}

/// <summary>
/// Gets the inventory component
/// </summary>
/// <returns></returns>
UPlayerInventoryComponent* APlayerCharacter::GetInventoryComponent()
{
	return InventoryComp;
}

/// <summary>
/// Gets the player stats component
/// </summary>
/// <returns></returns>
UStatsComponent* APlayerCharacter::GetStatsComponent()
{
	return StatsComp;
}

/// <summary>
/// Gets the player's interactable component
/// </summary>
/// <returns></returns>
UInteractableComponent* APlayerCharacter::GetInteractionComponent()
{
	return InteractionComp;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CHECK(InteractionComp);

	InteractionComp->RegisterComponent();

	CHECK(ItemSlotContainerClass);

	TObjectPtr<APlayerController> pPlayerController = GetWorld()->GetFirstPlayerController();
	CHECK(pPlayerController);

	InventoryUI = CreateWidget<UItemSlotContainer>(pPlayerController, ItemSlotContainerClass);
	CHECK(InventoryUI);

	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	InventoryUI->AddToViewport(3);
	InventoryUI->Setup(*InventoryComp);

	GameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(this));
	CHECK(GameInstance);

	GameInstance->InitializeSaveGameSlots(GetWorld());

	CHECK(PauseMenuClass);
	PauseMenuUI = CreateWidget<UPauseMenu>(pPlayerController, PauseMenuClass);
	PauseMenuUI->SetVisibility(ESlateVisibility::Hidden);
	PauseMenuUI->AddToViewport(4);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TObjectPtr<APlayerController> pPlayerController = Cast<APlayerController>(GetController());
	CHECK(pPlayerController);

	TObjectPtr<UEnhancedInputComponent> pInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	CHECK(pInputComponent);

	// Bind inputs
	CHECK(InputActions);
	pInputComponent->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	pInputComponent->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	pInputComponent->BindAction(InputActions->InputMap, ETriggerEvent::Triggered, this, &APlayerCharacter::TriggerMap);
	pInputComponent->BindAction(InputActions->InputJournal, ETriggerEvent::Triggered, this, &APlayerCharacter::TriggerJournal);
	pInputComponent->BindAction(InputActions->InputFlashlight, ETriggerEvent::Triggered, this, &APlayerCharacter::TriggerFlashlight);
	pInputComponent->BindAction(InputActions->InputConfirm, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
	pInputComponent->BindAction(InputActions->InputInventory, ETriggerEvent::Triggered, this, &APlayerCharacter::TriggerInventory);
	pInputComponent->BindAction(InputActions->InputConsume, ETriggerEvent::Triggered, this, &APlayerCharacter::Consume);
	pInputComponent->BindAction(InputActions->InputQuickSave, ETriggerEvent::Triggered, this, &APlayerCharacter::OnQuickSave);
	pInputComponent->BindAction(InputActions->InputQuickLoad, ETriggerEvent::Triggered, this, &APlayerCharacter::OnQuickLoad);
	pInputComponent->BindAction(InputActions->InputPause, ETriggerEvent::Triggered, this, &APlayerCharacter::OnTogglePauseMenu);

	// Get the local player subsystem
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> pSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pPlayerController->GetLocalPlayer());
	CHECK(pSubsystem);

	// Add our mapping
	pSubsystem->AddMappingContext(InputMapping, 0);
}

/// <summary>
/// Controls character movement
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
	if (!bCanMove)
	{
		return;
	}
	
	const FVector2D MoveValue = InputValue.Get<FVector2D>();
	const FRotator MovementRotation = FRotator(0, Controller->GetControlRotation().Yaw, 0);

	// Forward/Backward direction
	if (MoveValue.Y != 0.f)
	{
		// Get forward vector
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(Direction, MoveValue.Y);
	}

	// Right/Left direction
	if (MoveValue.X != 0.f)
	{
		// Get right vector
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(Direction, MoveValue.X);
	}
}

/// <summary>
/// Controls character turning/looking
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	if (!bCanLook)
	{
		return;
	}

	const FVector2D LookValue = InputValue.Get<FVector2D>();

	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}

	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(-1 * LookValue.Y);
	}
}

/// <summary>
/// Triggered when character attempts to interact with environment
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::Interact(const FInputActionValue& InputValue)
{
	CHECK(InteractionComp);

	if (bCanInteract)
	{
		InteractionComp->OnInteractTriggered();
	}

}

/// <summary>
/// Triggered when character attempts to consume an object on the ground
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::Consume(const FInputActionValue& InputValue)
{
	CHECK(InteractionComp);

	if (bCanInteract)
	{
		InteractionComp->OnConsumeTriggered();
	}
}

/// <summary>
/// Triggered when character opens/closes the map via the 'm' key
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::TriggerMap(const FInputActionValue& InputValue)
{
	OnMapPress();
}

/// <summary>
/// Triggered when character opens/closes the journal via the 'j' key
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::TriggerJournal(const FInputActionValue& InputValue)
{
	OnJournalPress();
}

/// <summary>
/// Triggered when character starts/turns off the flashlight via the 'f' key
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::TriggerFlashlight(const FInputActionValue& InputValue)
{
	OnFlashlightPress();
}

/// <summary>
/// Triggered when character opens/closes the inventory via the 'i' key
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::TriggerInventory(const FInputActionValue& InputValue)
{
	CHECK(InventoryComp);

	if (InventoryComp->bCanUseInventory)
	{
		CHECK(InventoryUI);

		bIsInventoryOpen = !bIsInventoryOpen;
		ESlateVisibility targetVisibility = bIsInventoryOpen ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		InventoryUI->SetVisibility(targetVisibility);

		if (targetVisibility == ESlateVisibility::Visible)
		{
			FInputModeGameAndUI inputMode;
			inputMode.SetWidgetToFocus(InventoryUI->TakeWidget());
			GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
		else if (targetVisibility == ESlateVisibility::Hidden)
		{
			FInputModeGameOnly inputMode;
			GetWorld()->GetFirstPlayerController()->SetInputMode(inputMode);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			InventoryUI->HandleOnCloseSlotContainer();
		}
	}
}

/// <summary>
/// Saves the current state of the game
/// Note: Quick Save slot is always 0
/// </summary>
void APlayerCharacter::OnQuickSave()
{
	CHECK(GameInstance);

	GameInstance->OnSaveGame(0);
}

/// <summary>
/// Loads the current state of the game
/// Note: Quick Load slot is always 0
/// </summary>
void APlayerCharacter::OnQuickLoad()
{
	CHECK(GameInstance);

	GameInstance->OnLoadGame(0);

}

/// <summary>
/// Toggles the pause menu
/// </summary>
/// <param name="InputValue"></param>
void APlayerCharacter::OnTogglePauseMenu(const FInputActionValue& InputValue)
{
	CHECK(PauseMenuUI);

	PauseMenuUI->TogglePauseMenu();
}
