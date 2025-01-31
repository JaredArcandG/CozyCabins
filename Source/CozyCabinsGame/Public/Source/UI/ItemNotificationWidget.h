// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TimerHandle.h"
#include "ItemNotificationWidget.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> NotificationMessage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification Settings", meta = (Text = "Amount of time the notification is fully visible"))
	float fTimeActive = 2.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notification Settings", meta = (Text = "Amount of time it should take for the notification to fade out"))
	float fTimeToFadeOut = 0.5f;

protected:

	FTimerHandle hTimeActive;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

public:

	void Setup(const struct FItemNotification& ItemNotificationInfo);

protected:

	UFUNCTION()
	void OnEndTimeActive();

	UFUNCTION()
	void OnEndFadeOut();
	
};
