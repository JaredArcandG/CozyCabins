// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Player/PlayerCharacter.h"
#include "Source/Utils/CheckUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include <EnhancedInputSubsystems.h>
#include <Source/Player/Input/InputConfigData.h>
#include <EnhancedInputComponent.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bCanMove = true;
	bCanLook = true;

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
}

void APlayerCharacter::AllowPlayerMove(const bool& bCanMoveState)
{
	bCanMove = bCanMoveState;
}

void APlayerCharacter::AllowPlayerLook(const bool& bCanLookState)
{
	bCanLook = bCanLookState;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
