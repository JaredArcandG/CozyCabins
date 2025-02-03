// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Source/Notifications/INotification.h"
#include "BaseNotification.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API UBaseNotification : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Text = "Widget class that maps to the notification"))
	TSubclassOf<UUserWidget> WidgetNotificationClass;

public:

	TSubclassOf<UUserWidget> GetWidgetNotificationClass();

	void Setup(TSubclassOf<UUserWidget> InWidgetClass);

};
