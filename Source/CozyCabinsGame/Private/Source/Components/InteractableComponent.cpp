// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/InteractableComponent.h"
#include "Components/SphereComponent.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/Components/IInteractable.h>
#include <Source/Player/PlayerCharacter.h>

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractiveCollider"));

	ActorFindRadius = 100.f;
	RaycastFreqSecs = 1.f;
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	CHECK(CollisionSphere);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);

	GetWorld()->GetTimerManager().SetTimer(RaycastHandle, this, &UInteractableComponent::ApplyRaycast, RaycastFreqSecs, true, 0.0f);
	
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/// <summary>
/// Actions to take when the sphere component overlaps an actor
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void UInteractableComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CHECK(OverlappedComponent);
	CHECK(OtherActor);

	if (OtherActor->Implements<UInteractable>())
	{
		OverlappedInteractableActors.AddUnique(*OtherActor);
	}
}

/// <summary>
/// Actions to take when the sphere component ends overlapping an actor
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
void UInteractableComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CHECK(OverlappedComponent);
	CHECK(OtherActor);

	if (OtherActor->Implements<UInteractable>())
	{
		OverlappedInteractableActors.Remove(*OtherActor);

		if (TargetActor == OtherActor)
		{
			TargetActor = nullptr;
		}
	}
}

/// <summary>
/// OnInteract
/// </summary>
void UInteractableComponent::OnInteractTriggered()
{
	CHECK(TargetActor);

	if (TargetActor->Implements<UInteractable>())
	{ 
		IInteractable::Execute_OnInteract(TargetActor, GetWorld(), Cast<ACharacter>(GetOwner()));
	}
}

/// <summary>
/// Applies a raycast to the environment at a certain distance
/// If the raycast hits a valid overlapped actor, we select it as the "target actor"
/// There is only one "target actor" at a time that the player can interact with
/// </summary>
void UInteractableComponent::ApplyRaycast()
{
	if (OverlappedInteractableActors.IsEmpty())
	{
		return;
	}

	// TODO:
	// 1. Raycast based on the actor find radius
	// 2. If actor found && actor in overlapped actors, set as target actor
	CHECK(GetOwner());

	FVector vRayStart = GetOwner()->GetActorLocation();
	FVector vForward = GetOwner()->GetActorForwardVector();
	FVector vRayEnd = vRayStart + (vForward * ActorFindRadius);

	FHitResult HitResult; 
	FCollisionQueryParams CollisionParams; 
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, vRayStart, vRayEnd, ECC_Visibility, CollisionParams);

	if (OverlappedInteractableActors.Contains(HitResult.GetActor()))
	{
		TargetActor = HitResult.GetActor();
	}

}

