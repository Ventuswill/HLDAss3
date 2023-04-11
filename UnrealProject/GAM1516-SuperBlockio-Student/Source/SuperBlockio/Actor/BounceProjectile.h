// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BounceProjectile.generated.h"

UCLASS()
class SUPERBLOCKIO_API ABounceProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABounceProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    /* Custom Components and Vairables*/
protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
        class USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* BallSprite;

    /* projectile movement component  */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
        class UProjectileMovementComponent* ProjectileMovementComponent;

    //Initial Speed of the Ball
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball", meta = (AllowPrivateAccess = "true"))
        float Speed = 8000;

private:

    FVector LaunchDir;


    UFUNCTION()
        void OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:

    void ShootThatProjectile(const FVector& Direction);

};
