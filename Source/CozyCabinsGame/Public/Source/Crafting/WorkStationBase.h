// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Source/Components/IInteractable.h"
#include "WorkStationBase.generated.h"

UCLASS()
class COZYCABINSGAME_API AWorkStationBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorkStationBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StationMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<class UBoxComponent> CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UWorkStationUIBase> StationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UDataTable> DataTable;

	UFUNCTION(BlueprintCallable)
	void DisplayWidget(UWorld* World, ACharacter* SourceCharacter);

	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	//void OnInteract(UWorld* World, ACharacter* SourceCharacter);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWorkStationUIBase> StationWidget;
};
