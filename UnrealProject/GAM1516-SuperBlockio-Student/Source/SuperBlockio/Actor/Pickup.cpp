// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include <Components/SphereComponent.h>
#include "PaperSpriteComponent.h"

#include "Helpers/LockAxisfor2D.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    //CREATE the SphereCollision Component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    
	//CALL SetCollisionEnabled() on SphereCollision passing in ECollisionEnabled::QueryAndPhysics
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CALL SetCollisionProfileName() on SphereCollision passing in "OverlapAll"
    SphereCollision->SetCollisionProfileName("OverlapAll");
	//CALL SetGenerateOverlapEvents() on SphereCollision passing in  true
    SphereCollision->SetGenerateOverlapEvents(true);
	//CALL SetSimulatePhysics() on SphereCollision passing in false
    SphereCollision->SetSimulatePhysics(false);
	//SET the RootComponent to SphereCollision
    RootComponent = SphereCollision;

	//CREATE the Sprite Component
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	//CALL SetCollisionEnabled() on Sprite passing in ECollisionEnabled::NoCollision
    Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CALL SetCollisionProfileName() on Sprite passing in "NoCollision"
    Sprite->SetCollisionProfileName("NoCollision");
	//CALL SetupAttachment() on Sprite passing in RootComponent
    Sprite->SetupAttachment(RootComponent);

	//CALL Add() on Tags passing in "Pickup"
    Tags.Add("Pickup");
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

