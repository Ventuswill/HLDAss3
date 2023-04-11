// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SuperBlockioGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SUPERBLOCKIO_API ASuperBlockioGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
    ASuperBlockioGameStateBase();

    //Called when Game Starts
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
        int32 GetScore();

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
        uint8 GetLives();

    UFUNCTION(BlueprintCallable, Category = "Player Stats")
        int32 GetCoins();

    void SetCheckpointLocation(FVector Location);
    FVector GetCheckpointLocation();

    void SwitchONOFFblocks();

    void IncreaseLives(int value);
    void IncreaseCoins(int value);
    void IncreaseScore(int value);

protected:
    //Lives
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
        uint8 PlayerLives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
        int32 PlayerScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
        int32 PlayerCoins;

    FVector RespawnLocation;

private:
	
};
