// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COZYCABINSGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APlayerCharacter();

	// Follow camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;

protected:

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	// Scales turning functionality
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Enhanced Input")
	float BaseTurnRate = 65.f;

	// Scales look up functionality
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Enhanced Input")
	float BaseLookupRate = 65.f;

	// The type of input mapping for the character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<class UInputMappingContext> InputMapping;

	// Pointer to the input actions of the character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<class UInputConfigData> InputActions;

	// Handles interaction with the environment
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UInteractableComponent> InteractionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UInventoryComponent> InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStatsComponent> StatsComp;

	// Whether the character is allowed to move or not
	bool bCanMove;

	// Whether the character can look or not
	bool bCanLook;

	// Whether character can interact or not
	bool bCanInteract;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnMapPress();

	UFUNCTION(BlueprintImplementableEvent)
	void OnJournalPress();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFlashlightPress();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Allow Player Move", ToolTip = "Enables or disables the player's ability to move."))
	void AllowPlayerMove(const bool& bCanMoveState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Allow Player Look", ToolTip = "Enables or disables the player's ability to look."))
	void AllowPlayerLook(const bool& bCanLookState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Allow Player Look", ToolTip = "Enables or disables the player's ability to look."))
	void AllowPlayerInteract(const bool& bCanInteractState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Inventory Component", ToolTip = "Gets the player's inventory."))
	class UInventoryComponent* GetInventoryComponent();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Stats Component", ToolTip = "Gets the player's stats"))
	class UStatsComponent* GetStatsComponent();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle move input
	void Move(const FInputActionValue& InputValue);

	// Handle look input
	void Look(const FInputActionValue& InputValue);

	// Handle interact input
	void Interact(const FInputActionValue& InputValue);

	// Handle show/hide map
	void TriggerMap(const FInputActionValue& InputValue);

	// Handle show/hide journal
	void TriggerJournal(const FInputActionValue& InputValue);

	// Handle start/turn off flashlight
	void TriggerFlashlight(const FInputActionValue& InputValue);

	// Handle show/hide inventory
	void TriggerInventory(const FInputActionValue& InputValue);

};
