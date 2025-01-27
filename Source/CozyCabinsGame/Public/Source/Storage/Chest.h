// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Source/Components/IInteractable.h"
#include "Chest.generated.h"

UCLASS()
class COZYCABINSGAME_API AChest : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AChest();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UChestUI> ChestUIClass;

	UPROPERTY()
	TObjectPtr<class UChestUI> ChestUIWidget;

	bool bIsChestInventoryOpen;

	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteract_Implementation(UWorld* World, ACharacter* SourceCharacter) override;

	void OpenChestUIWidget();

	void CloseChestUIWidget();



};
