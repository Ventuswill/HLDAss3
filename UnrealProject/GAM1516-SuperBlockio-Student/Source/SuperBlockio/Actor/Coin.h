// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Pickup.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBLOCKIO_API ACoin : public APickup
{
	GENERATED_BODY()

public:
    ACoin();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
	
};
