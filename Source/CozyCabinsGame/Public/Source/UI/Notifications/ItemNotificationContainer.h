// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNotificationContainer.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemNotificationContainer : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UVerticalBox> NotificationContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UItemNotificationWidget> ItemNotificationClass;

protected:

	UPROPERTY()
	TObjectPtr<class ACustomPlayerController> PlayerController;

public:

	virtual void NativeConstruct() override;

	void AddNotificationToContainer(UUserWidget& ChildToAdd);

protected:

	UFUNCTION()
	void OnNotificationReceived(class UBaseNotification* Notification);
};
