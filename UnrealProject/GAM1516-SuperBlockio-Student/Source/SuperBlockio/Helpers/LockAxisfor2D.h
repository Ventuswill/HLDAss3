// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LockAxisfor2D.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBLOCKIO_API ULockAxisfor2D : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    ULockAxisfor2D();

    static void LockPhysicsTo2DAxis(class UPrimitiveComponent* primitive);
};
