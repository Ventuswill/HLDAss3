// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/ONOFFBlockComponent.h"

#include "ONOFFBlock.generated.h"

UCLASS()
class SUPERBLOCKIO_API AONOFFBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AONOFFBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ONOFF", meta = (AllowPrivateAccess = "true"))
        class UBoxComponent* BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ONOFF", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* BlockSprite;

    //Weapon Inventory Component
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UONOFFBlockComponent* OnOffComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ONOFF")
        EBlockOnOff BlockType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    EBlockOnOff GetType();
    void SwitchStates();

    FTimerHandle ReactivationTimer;

private:
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    void ReactivationDelay();
    bool Activated;

};
