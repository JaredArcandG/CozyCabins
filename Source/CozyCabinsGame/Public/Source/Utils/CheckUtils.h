// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * Utils for check macros (for readability)
 */
struct CheckUtils
{
#define CHECK(pObj) \
        do { \
            if (!pObj) { \
                UE_LOG(LogTemp, Error, TEXT("Null pointer detected in file %s | line %d"), ANSI_TO_TCHAR(__FILE__), __LINE__); \
                return; \
            } \
        } while (false)
};
