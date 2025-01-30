// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Source/Components/IInteractable.h"
#include "Planter.generated.h"

UCLASS()
class COZYCABINSGAME_API APlanter : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> PlanterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UInventoryComponent> InventoryComponent;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGameTimeManager> GameTimeManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPlanterUI> PlanterUIWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class ACrop> CropClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlanterUI> PlanterUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDataTable> ItemsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TObjectPtr<USceneComponent>, TObjectPtr<AActor>> FarmingSpots;


	//// ------- Functions ------- ////

	UFUNCTION(BlueprintCallable)
	void DisplayWidget(UWorld* World, ACharacter* SourceCharacter);

	UFUNCTION(BlueprintCallable)
	void UpdateTimeOfCrops(FTimespan PassedTime);

	UFUNCTION(BlueprintCallable)
	void PlantSeed(USceneComponent* FarmSpot, FItemData ItemData);

	UFUNCTION(BlueprintCallable)
	void CheckAndFillEmptySlots();

	UFUNCTION(BlueprintCallable)
	void CropFinished(USceneComponent* FarmSpot, FItemData ItemData, AActor* CropActor);

	UFUNCTION(BlueprintCallable)
	void TimePassed(FTimespan PassedTime);

	//// ------- Debug ------- ////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDataTableRowHandle> DebugItems;

	UFUNCTION(BlueprintCallable)
	void DebugTest();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
};
