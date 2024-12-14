// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COZYCABINSGAME_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UInteractableComponent();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USphereComponent> CollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ToolTip = "How far in units the component can detect the target"))
	float ActorFindRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ToolTip = "How frequently to perform the raycast checks. Increase time to improve performance."))
	float RaycastFreqSecs = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ToolTip = "How far in units the component can interact with the target"))
	float ActorInteractRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast Settings", meta = (ToolTip = "Actor classes to ignore for the raycast"))
	TArray<TSubclassOf<AActor>> IgnoredActorClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Settings", meta = (ToolTip = "Enable debug mode to see interaction/collision debug lines"))
	bool bEnableDebugMode;

protected:

	FTimerHandle RaycastHandle;

	TArray<TObjectPtr<AActor>> OverlappedInteractableActors;

	// Should only have one actor at a time
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void ApplyRaycast();

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnInteractTriggered();
};
