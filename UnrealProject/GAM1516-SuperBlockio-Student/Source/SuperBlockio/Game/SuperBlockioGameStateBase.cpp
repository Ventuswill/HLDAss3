// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperBlockioGameStateBase.h"

#include "Pawns/PlayerCharacter.h"
#include "Actor/ONOFFBlock.h"
#include "EngineUtils.h"

ASuperBlockioGameStateBase::ASuperBlockioGameStateBase()
{
	//SET PlayerLives to 5
    PlayerLives = 5;
    
	//SET PlayerScore to 0
    PlayerScore = 0;
    
	//SET PlayerCoins to 0
    PlayerCoins = 0;
}

void ASuperBlockioGameStateBase::BeginPlay()
{
    Super::BeginPlay();
}

FVector ASuperBlockioGameStateBase::GetCheckpointLocation()
{
    return RespawnLocation;
}

void ASuperBlockioGameStateBase::SetCheckpointLocation(FVector Location)
{
    RespawnLocation = Location;
}


uint8 ASuperBlockioGameStateBase::GetLives()
{
    return PlayerLives;
}

int32 ASuperBlockioGameStateBase::GetCoins()
{
    return PlayerCoins;
}


int32 ASuperBlockioGameStateBase::GetScore()
{
    return PlayerScore;
}

void ASuperBlockioGameStateBase::IncreaseLives(int value)
{
    PlayerLives += value;
}

void ASuperBlockioGameStateBase::IncreaseCoins(int value)
{
    PlayerCoins += value;
}

void ASuperBlockioGameStateBase::IncreaseScore(int value)
{
    PlayerScore += value;
}


void ASuperBlockioGameStateBase::SwitchONOFFblocks()
{
	//FOR TActorIterator<AONOFFBlock> OnOffBlockItr(GetWorld()); OnOffBlockItr; ++OnOffBlockItr
    for (TActorIterator<AONOFFBlock> OnOffBlockItr(GetWorld()); OnOffBlockItr; ++OnOffBlockItr)
    {
    	//CALL SwitchStates() on OnOffBlockItr
        OnOffBlockItr->SwitchStates();
    }
	//ENDFOR
}





