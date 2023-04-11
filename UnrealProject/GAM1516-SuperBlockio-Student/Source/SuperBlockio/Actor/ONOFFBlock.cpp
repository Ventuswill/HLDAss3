// Fill out your copyright notice in the Description page of Project Settings.


#include "ONOFFBlock.h"

#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Components/BoxComponent.h"

#include "Game/SuperBlockioGameStateBase.h"

// Sets default values
AONOFFBlock::AONOFFBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    //SET BlockType to EBlockOnOff::Trigger
    BlockType = EBlockOnOff::Trigger;
    

	//CREATE the BoxCollision Component
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    
	//CALL SetCollisionProfileName() on BoxCollision passing in "BlockAll"
    BoxCollision->SetCollisionProfileName("BlockAll");
    
	//CALL  BoxCollision->OnComponentHit.AddDynamic() passing in this, &AONOFFBlock::OnHit
    BoxCollision->OnComponentHit.AddDynamic(this, &AONOFFBlock::OnHit);
    
	//SET RootComponent to BoxCollision
    RootComponent = BoxCollision;

	//CREATE the BlockSprite Component
    BlockSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BlockSprite"));
	//CALL SetCollisionProfileName() on BlockSprite passing in "NoCollision"
    BlockSprite->SetCollisionProfileName("NoCollision");
	//CALL SetCollisionEnabled() on BlockSprite passing in ECollisionEnabled::NoCollision
    BlockSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CALL SetupAttachment() on BlockSprite passing in RootComponent
    BlockSprite->SetupAttachment(RootComponent);

	//CREATE the OnOffComponent
    OnOffComponent = CreateDefaultSubobject<UONOFFBlockComponent>(TEXT("OnOffComponent"));

	//SET Activated to false
    Activated = false;
}

// Called when the game starts or when spawned
void AONOFFBlock::BeginPlay()
{
	Super::BeginPlay();

	//IF BlockType IS EBlockOnOff::OnOff
    if (BlockType == EBlockOnOff::OnOff)
    {
    
    	//CALL BoxCollision->OnComponentHit.RemoveDynamic() passing in this, &AONOFFBlock::OnHit
        BoxCollision->OnComponentHit.RemoveDynamic(this, &AONOFFBlock::OnHit);
    
    }
	//ENDIF
}

// Called every frame
void AONOFFBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EBlockOnOff AONOFFBlock::GetType()
{
    return BlockType;
}

void AONOFFBlock::SwitchStates()
{
	//CALL SwitchStates() on OnOffComponent passing in this
    OnOffComponent->SwitchStates(this);
    
	//IF BlockType IS EBlockOnOff::OnOff
    if (BlockType == EBlockOnOff::OnOff)
    {
    	//IF OnOffComponent->GetState()
        if (OnOffComponent->GetState())
        {
        	//CALL SetSprite() on BlockSprite passing in --> Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_SwitchON_Sprite")))
            BlockSprite->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_SwitchON_Sprite"))));
        }
    	//ELSE
        else
        {
        	//CALL SetSprite() on BlockSprite passing in --> StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_SwitchOFF_Sprite")))
            BlockSprite->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_SwitchOFF_Sprite"))));
        }
    	//ENDIF
    }
	//ELSE IF BlockType IS EBlockOnOff::Trigger
    else if (BlockType == EBlockOnOff::Trigger)
    {
    	//IF OnOffComponent->GetState()
        if (OnOffComponent->GetState())
        {
        	//CALL SetSprite() on BlockSprite passing in --> StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_TriggerON_Sprite")))
            BlockSprite->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_TriggerON_Sprite"))));
        }
    	//ELSE
        else
        {
        	//CALL SetSprite() on BlockSprite passing in --> StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_TriggerOFF_Sprite")))
            BlockSprite->SetSprite(Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), NULL, TEXT("/Game/Textures/8Bit_ONOFF_TriggerOFF_Sprite"))));
        }
    	//ENDIF
    }
	//ENDIF
}

void AONOFFBlock::ReactivationDelay()
{
	//SET Activated to false
    Activated = false;
	//CALL GetWorldTimerManager().ClearTimer() passing in ReactivationTimer
    GetWorldTimerManager().ClearTimer(ReactivationTimer);
}



//This is only for the Switch Block, the Solid or Lined Block has no overlap other than a blocking
void AONOFFBlock::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//DECLARE a variable called gamestate of type ASuperBlockioGameStateBase* and assign it to the return value of GetWorld()->GetGameState<ASuperBlockioGameStateBase>()
    ASuperBlockioGameStateBase* gamestate = GetWorld()->GetGameState<ASuperBlockioGameStateBase>();

	//IF gamestate NOT nullptr
    if (gamestate)
    {
    	//IF OtherActor->ActorHasTag("Player") AND NOT Activated AND BlockType IS EBlockOnOff::Trigger
       if (OtherActor->ActorHasTag("Player") && !Activated && BlockType == EBlockOnOff::Trigger)
        {
            /*Ideally, if hit from below*/
        	//IF Hit.ImpactPoint.Z LESS THAN EQUAL to (BoxCollision->GetComponentLocation().Z - (BoxCollision->GetScaledBoxExtent().Z/2))
           if (Hit.ImpactPoint.Z <= (BoxCollision->GetComponentLocation().Z - (BoxCollision->GetScaledBoxExtent().Z / 2)))
            {
            	//SET Activated to true
                 Activated = true;
            	//CALL SwitchONOFFblocks() on gamestate
                 gamestate->SwitchONOFFblocks();
            }
        }
    	//ELSE IF OtherActor->ActorHasTag("Ball") AND NOT Activated AND BlockType == EBlockOnOff::Trigger
        else if (OtherActor->ActorHasTag("Ball") && !Activated && BlockType == EBlockOnOff::Trigger)
        {
            /*If it was hit by projectile, then anything goes. Try to make it so only player's projectile can turn ONOFF*/
        	//IF OtherActor->GetOwner()->ActorHasTag("Player")
           if (OtherActor->GetOwner()->ActorHasTag("Player"))
            {
            	//SET Activated to true
                Activated = true;
            	//CALL SwitchONOFFblocks() on gamestate
                gamestate->SwitchONOFFblocks();
            	//CALL Destroy() on OtherActor
                OtherActor->Destroy();
            }
        	//ENDIF
        }
    	//ENDIF

    	//IF NOT GetWorldTimerManager().IsTimerActive(ReactivationTimer)
       if (!GetWorldTimerManager().IsTimerActive(ReactivationTimer))
        {
        	//CALL GetWorldTimerManager().SetTimer() passing in ReactivationTimer, this, &AONOFFBlock::ReactivationDelay, .5, false
            GetWorldTimerManager().SetTimer(ReactivationTimer, this, &AONOFFBlock::ReactivationDelay, .5, false);
        }
    	//ENDIF
    }
	//ENDIF

}