// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"


#include "SuperBlockioGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBLOCKIO_API ASuperBlockioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    ASuperBlockioGameModeBase();

    //Called when Game Starts
    virtual void BeginPlay() override;

    void OnDeath();
    void OnCheckpoint(FVector FlagPos);

    UFUNCTION(BlueprintNativeEvent, Category = "Blockio")
        void OnGoal();

    UFUNCTION(BlueprintNativeEvent, Category = "Blockio")
        void OnGameOver();

    void Goal();
    void GameOver();
protected:

private:

    class APlayerCharacter* PlayerCharacter;
    class ASuperBlockioGameStateBase* GameState;
};
