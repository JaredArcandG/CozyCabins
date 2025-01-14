// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Timespan.h"
#include "Source/Items/ItemData.h"
#include "Crop.generated.h"

class APlanter;
class UGameTimeManager;

UCLASS()
class COZYCABINSGAME_API ACrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> CropMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UGameTimeManager> GameTimeManager;

	UPROPERTY(VisibleAnywhere)
	ECropStage CurrentCropStage;

	UPROPERTY(VisibleAnywhere)
	FDateTime TimePlanted;

	UPROPERTY(VisibleAnywhere)
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlanter> ParentPlanter;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FarmSpot;

	UFUNCTION(BlueprintCallable)
	void InitCrop(FItemData ItemDataInput, APlanter* Planter, UGameTimeManager* TimeManager, USceneComponent* SceneComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateTime();

	UFUNCTION(BlueprintCallable)
	ECropStage CheckCropStage(FTimespan TimePassed);

	UFUNCTION(BlueprintCallable)
	void UpdateCropStage(ECropStage NewCropStage);

	UFUNCTION(BlueprintCallable)
	void HandleCropFinish();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
