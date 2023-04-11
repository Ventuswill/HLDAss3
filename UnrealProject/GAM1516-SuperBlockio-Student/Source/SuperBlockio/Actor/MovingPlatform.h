// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"


UENUM(BlueprintType)
enum class ETarget : uint8
{
    First UMETA(DisplayName = "First Target"),
    Second UMETA(DisplayName = "Second Target"),
};

UCLASS()
class SUPERBLOCKIO_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Pathing")
        class UBoxComponent* Target1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Pathing")
        UBoxComponent* Target2;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Collision")
         UBoxComponent* BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* PlatformSprite;

    FVector Target1Location;
    FVector Target2Location;

    FVector CurrentGoToLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing")
        ETarget StartingTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing")
        float InterpSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
