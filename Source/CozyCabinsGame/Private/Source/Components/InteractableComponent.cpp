// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Components/InteractableComponent.h"
#include "Components/SphereComponent.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/Components/IInteractable.h>
#include <Source/Player/PlayerCharacter.h>
#include "Camera/CameraComponent.h"
#include "Source/Constants/TraceChannel.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractiveCollider"));
	
	CHECK(CollisionSphere);
	CollisionSphere->SetSphereRadius(ActorFindRadius);

	bEnableDebugMode = false;
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	CHECK(CollisionSphere);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnOverlapEnd);

	CollisionSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	GetWorld()->GetTimerManager().SetTimer(RaycastHandle, this, &UInteractableComponent::ApplyRaycast, RaycastFreqSecs, true, 0.0f);
	
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableDebugMode)
	{
		CHECK(CollisionSphere);
		FVector SphereLocation = CollisionSphere->GetComponentLocation(); // Get the current location of the sphere 

#ifdef UE_BUILD_DEBUG
		DrawDebugSphere(GetWorld(), SphereLocation, ActorFindRadius, 12, FColor::Green, false, -1, 0, 2);
#endif
	}
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

	TObjectPtr<APlayerCharacter> pCharacter = Cast<APlayerCharacter>(GetOwner());
	CHECK(pCharacter);
	CHECK(pCharacter->FollowCamera);

	ECollisionChannel interactableChannel = ECC_InteractableChannel;

	FVector vRayStart = pCharacter->FollowCamera->GetComponentLocation();
	FVector vForward = pCharacter->FollowCamera->GetForwardVector();
	FVector vRayEnd = vRayStart + (vForward * ActorInteractRadius);

	if (bEnableDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ray Start: %s, Ray End: %s"), *vRayStart.ToString(), *vRayEnd.ToString());
	}

	TArray<FHitResult> hitResults; 
	FCollisionQueryParams CollisionParams; 
	CollisionParams.AddIgnoredActor(GetOwner());

	for (TSubclassOf<AActor> ignoreClass : IgnoredActorClasses) 
	{ 
		for (TActorIterator<AActor> It(GetWorld(), ignoreClass); It; ++It)
		{
			TObjectPtr<AActor> pIgnoredActor = *It;
			CollisionParams.AddIgnoredActor(pIgnoredActor);
			
			if (bEnableDebugMode)
			{
				UE_LOG(LogTemp, Warning, TEXT("Ignoring Actor: %s"), *pIgnoredActor->GetName());
			}
		} 
	}
	

	GetWorld()->LineTraceMultiByChannel(hitResults, vRayStart, vRayEnd, interactableChannel, CollisionParams);

	if (bEnableDebugMode)
	{
#ifdef UE_BUILD_DEBUG
		DrawDebugLine(GetWorld(), vRayStart, vRayEnd, FColor::Blue, false, RaycastFreqSecs, 0, 1.0f);
#endif
	}

	bool bFoundActor = false;

	for (FHitResult hitResult : hitResults)
	{
		TObjectPtr<AActor> pHitActor = hitResult.GetActor();

		if (pHitActor)
		{
			if (bEnableDebugMode)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Class: %s, Location: %s, Component: %s"), *pHitActor->GetName(), *pHitActor->GetClass()->GetName(), *hitResult.ImpactPoint.ToString(), *hitResult.Component->GetName());
			}

			// Log collision response for each component 
			for (TObjectPtr<UActorComponent> Component : pHitActor->GetComponents()) 
			{
				TObjectPtr<UPrimitiveComponent> PrimitiveComponent = Cast<UPrimitiveComponent>(Component); 

				if (PrimitiveComponent)
				{ 
					ECollisionResponse Response = PrimitiveComponent->GetCollisionResponseToChannel(ECC_InteractableChannel);

					if (bEnableDebugMode)
					{
						UE_LOG(LogTemp, Warning, TEXT("Component: %s, CollisionResponse to Interactable: %d"), *PrimitiveComponent->GetName(), Response);
					}
				} 
			}
		}

		if (pHitActor && OverlappedInteractableActors.Contains(pHitActor))
		{
			bFoundActor = true;
			TargetActor = pHitActor;
			break;
		}
	}

	if (!bFoundActor)
	{
		TargetActor = nullptr;
	}

}
