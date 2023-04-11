// Fill out your copyright notice in the Description page of Project Settings.


#include "ONOFFBlockComponent.h"
#include "GameFramework/Actor.h"
#include "Actor/ONOFFBlock.h"

// Sets default values for this component's properties
UONOFFBlockComponent::UONOFFBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//SET State to true
    State = true;
    
}


// Called when the game starts
void UONOFFBlockComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UONOFFBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UONOFFBlockComponent::GetState()
{
    return State;
}

void UONOFFBlockComponent::SwitchStates(AONOFFBlock* Block)
{
	//IF State IS true
    if (State)
    {
    	//IF Block->GetType() IS EBlockOnOff::OnOff
        if (Block->GetType() == EBlockOnOff::OnOff)
        {
        	//CALL SetActorEnableCollision() on Block passing in false
            Block->SetActorEnableCollision(false);
        }
    	//ENDIF

    	//SET State to false
        State = false;
    }
	//ELSE
    
    {
    	//IF Block->GetType() IS EBlockOnOff::OnOff
        if (Block->GetType() == EBlockOnOff::OnOff)
        {
        	//CALL SetActorEnableCollision() on Block passing in true
            Block->SetActorEnableCollision(true);
        }
    	//ENDIF

    	//SET State to true
        State = true;
    }
	//ENDIF
}


