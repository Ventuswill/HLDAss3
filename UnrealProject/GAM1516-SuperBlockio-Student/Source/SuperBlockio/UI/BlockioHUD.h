// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlockioHUD.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBLOCKIO_API ABlockioHUD : public AHUD
{
	GENERATED_BODY()

public:
    ABlockioHUD();

    UPROPERTY(EditAnywhere, Category = "Game Hud")
        TSubclassOf<class UUserWidget> BlockioGUIClass;

    UPROPERTY()
        class UUserWidget* BlockioGUI;

protected:
    virtual void BeginPlay() override;
	
};
