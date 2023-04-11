// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"



#include "Flagpoint.generated.h"

UENUM(BlueprintType)
enum class EFlagEnum : uint8
{
    Checkpoint UMETA(DisplayName = "Checkpoint"),
    Goal UMETA(DisplayName = "Goal")
};

UCLASS()
class SUPERBLOCKIO_API AFlagpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
        class UBoxComponent* BoxCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* FlagSprite;

    //Weapon Inventory Component
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        class UFlagComponent* FlagComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
        EFlagEnum Flagtype;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
