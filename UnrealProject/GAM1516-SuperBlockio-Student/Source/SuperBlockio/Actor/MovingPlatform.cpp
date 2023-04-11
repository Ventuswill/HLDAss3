// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SET StartingTarget to ETarget::First
    StartingTarget = ETarget::First;
    
    //CREATE Target1 Component
    Target1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Target1"));
    
	//CALL SetCollisionProfileName() on Target1 passing in "NoCollision"
    Target1->SetCollisionProfileName("NoCollision");
    
	//CALL SetCollisionEnabled() on Target1 passing in ECollisionEnabled::NoCollision
    Target1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//CALL SetupAttachment() on Target1 passing in RootComponent
    Target1->SetupAttachment(RootComponent);

	//CREATE Target2 Component
    Target2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Target2"));
    
	//CALL SetCollisionProfileName() on Target2 passing in "NoCollision"
    Target2->SetCollisionProfileName("NoCollision");
    
	//CALL SetCollisionEnabled() on Target2 passing in ECollisionEnabled::NoCollision
    Target2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//CALL SetupAttachment() on Target2 passing in RootComponent
    Target2->SetupAttachment(RootComponent);
    

	//CREATE the BoxCollision Component
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    
	//CALL SetCollisionProfileName() on BoxCollision passing in "BlockAll"
    BoxCollision->SetCollisionProfileName("BlockAll");
    
	//SET RootComponent to BoxCollision
    RootComponent = BoxCollision;

	//CREATE the PlatformSprite Component
    PlatformSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlatformSprite"));
    
	//CALL SetCollisionProfileName() on PlatformSprite passing in "NoCollision"
    PlatformSprite->SetCollisionProfileName("NoCollision");
    
	//CALL SetCollisionEnabled() on PlatformSprite passing in ECollisionEnabled::NoCollision
    PlatformSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	//CALL SetupAttachment() on PlatformSprite passing in RootComponent
    PlatformSprite->SetupAttachment(RootComponent);
    

	//SET InterpSpeed to 600.0f
    InterpSpeed = 600.0f;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//SET Target1Location to the return value of Target1->GetComponentLocation()
    Target1Location = Target1->GetComponentLocation();
	//SET Target2Location to the return value of Target2->GetComponentLocation()
    Target2Location = Target2->GetComponentLocation();

	//IF StartingTarget IS ETarget::First
    if (StartingTarget == ETarget::First)
    {
    	//SET CurrentGoToLocation to Target1Location
        CurrentGoToLocation = Target1Location;
    }
	//ELSE
    else
    {
    	//SET CurrentGoToLocation to Target2Location
        CurrentGoToLocation = Target2Location;
    }
	//ENDIF
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DECLARE a variable called Delta of type FVector const and assign it to BoxCollision->GetComponentLocation() - CurrentGoToLocation
    FVector const Delta = BoxCollision->GetComponentLocation() - CurrentGoToLocation;
	//DECLARE a variable called DistanceToPoint and assign it to the return value of Delta.Size()
    float DistanceToPoint = Delta.Size();

	//IF DistanceToPoint LESS THAN 100
    if (DistanceToPoint < 100)
    {
    	//IF CurrentGoToLocation NOT EQUAL to Target1Location
        if (CurrentGoToLocation != Target1Location)
        {
        	//SET CurrentGoToLocation to Target1Location
            CurrentGoToLocation = Target1Location;
        }
    	//ELSE
        else
        {
        	//SET CurrentGoToLocation to Target2Location
            CurrentGoToLocation = Target2Location;
        }

    }
	//ENDIF

	//DECLARE a variable called targetLocation of type FVector and assign it to the return value of FMath::VInterpConstantTo(BoxCollision->GetComponentLocation(), CurrentGoToLocation, DeltaTime, InterpSpeed)
    FVector targetLocation = FMath::VInterpConstantTo(BoxCollision->GetComponentLocation(), CurrentGoToLocation, DeltaTime, InterpSpeed);
	//CALL SetWorldLocation() on BoxCollision passing in targetLocation
    BoxCollision->SetWorldLocation(targetLocation);
    

}

