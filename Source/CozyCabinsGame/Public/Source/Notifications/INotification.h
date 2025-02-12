// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Source/UI/Notifications/ItemNotificationContainer.h"
#include "INotification.generated.h"


 // This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNotification : public UInterface
{
	GENERATED_BODY()
};

class COZYCABINSGAME_API INotification
{
	GENERATED_BODY()

public:

	virtual void SetupNotification(class UBaseNotification& Notification);
};