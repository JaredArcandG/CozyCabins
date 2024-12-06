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

protected:

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	// Follow camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;

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

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handle move input
	void Move(const FInputActionValue& InputValue);

	// Handle look input
	void Look(const FInputActionValue& InputValue);

};
