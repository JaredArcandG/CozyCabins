// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/WorldDroppableItemWidget.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Player/PlayerCharacter.h"
#include "Source/Player/Controller/CustomPlayerController.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/UI/CustomDragDropOperation.h>
#include <Source/Factory/ItemFactory.h>
#include <Source/UI/ItemSlotDragPreview.h>
#include <Source/Items/Item.h>
#include "Source/Components/InteractableComponent.h"

void UWorldDroppableItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TObjectPtr<APlayerController> pController = GetWorld()->GetFirstPlayerController();
	CHECK(pController);

	PlayerRef = pController->GetPawn();
	CHECK(PlayerRef);

	PlayerInteractableCompRef = PlayerRef->GetComponentByClass<UInteractableComponent>();
}

/// <summary>
/// Handle case where an item slot is dropped into the world
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InDragDropEvent"></param>
/// <param name="InOperation"></param>
/// <returns></returns>
bool UWorldDroppableItemWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	TObjectPtr<UCustomDragDropOperation> pItemOperation = Cast<UCustomDragDropOperation>(InOperation);

	// Operation must be valid with the preview slot ref
	// Inventory should be valid
	// Player Interactable Component should be valid
	if (!pItemOperation || !pItemOperation->ItemSlotPreviewRef || !PlayerInteractableCompRef || !PlayerRef)
	{
		return false;
	}

	TObjectPtr<UWorld> pWorld = GetWorld();
	FGuid itemId = pItemOperation->ItemSlotPreviewRef->ItemId;
	int qty = pItemOperation->ItemSlotPreviewRef->Quantity;
	TSubclassOf<AItem> itemClass = pItemOperation->ItemSlotPreviewRef->ItemClass;
	int inventoryIdx = pItemOperation->ItemSlotPreviewRef->InventoryIdx;
	TObjectPtr<UInventoryComponent> pInventoryComp = pItemOperation->InventoryCompRef;

	// World must be valid
	// Inventory Component should exist
	// Item must be droppable
	if (!pWorld || !itemClass || !pInventoryComp || !pItemOperation->ItemSlotPreviewRef->bIsDroppable)
	{
		return false;
	}

	// Now that all values are valid, make sure to spawn the object in the same location as the player is facing
	float fDistanceToSpawn = PlayerInteractableCompRef->ActorInteractRadius;
	FVector vForwardVector = PlayerRef->GetActorForwardVector();
	FVector vLocationToSpawn = PlayerRef->GetActorLocation() + (vForwardVector * fDistanceToSpawn);
	FRotator vRotationToSpawn = PlayerRef->GetActorRotation();

	// Raycast so item is dropped on the ground
	FHitResult hitResult; 
	FVector vTraceStart = vLocationToSpawn;
	FVector vTraceEnd = vLocationToSpawn - FVector(0, 0, 10000);
	FCollisionQueryParams queryParams; 
	queryParams.AddIgnoredActor(PlayerRef); 
	bool bHitGround = pWorld->LineTraceSingleByChannel(hitResult, vTraceStart, vTraceEnd, ECC_Visibility, queryParams);
	
	if (bHitGround) 
	{ 
		vLocationToSpawn.Z = hitResult.Location.Z;
	}

	FTransform itemTransform = FTransform();
	itemTransform.SetLocation(vLocationToSpawn);
	itemTransform.SetRotation(vRotationToSpawn.Quaternion());

	if (UItemFactory::SpawnItemInWorld(*pWorld, itemTransform, itemClass, itemId, qty))
	{
		return pInventoryComp->TryRemoveAtIndex(itemId, inventoryIdx, qty);
	}

	return false;
}
