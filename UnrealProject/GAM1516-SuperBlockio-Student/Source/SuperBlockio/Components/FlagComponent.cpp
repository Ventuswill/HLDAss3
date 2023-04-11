// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagComponent.h"

#include "Game/SuperBlockioGameModeBase.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UFlagComponent::UFlagComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//SET isActivated to false
    isActivated = false;
}


// Called when the game starts
void UFlagComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UFlagComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFlagComponent::OnCheckpoint(FVector FlagPos)
{
	//CREATE a variable called Gamemode of type ASuperBlockioGameModeBase* and assign it to the return of Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode())
    ASuperBlockioGameModeBase* Gamemode = Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode());

	//IF Gamemode NOT nullptr
    if (Gamemode)
    {
    	//CALL OnCheckpoint() on Gamemode passing in FlagPos
        Gamemode->OnCheckpoint(FlagPos);
    	//SET isActivated to true
        isActivated = true;
    }
	//ENDIF
}

bool UFlagComponent::CheckpointHasBeenReached()
{
    return isActivated;
}


void UFlagComponent::OnGoal()
{
	//SET isActivated to true
    isActivated = true;

    //CREATE a variable called Gamemode of type ASuperBlockioGameModeBase* and assign it to the return of Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode())
    ASuperBlockioGameModeBase* Gamemode = Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode());

	//IF Gamemode NOT nullptr
    if (Gamemode)
    {
    	//CALL Goal() on Gamemode
        Gamemode->Goal();
    }
	//ENDIF
}


