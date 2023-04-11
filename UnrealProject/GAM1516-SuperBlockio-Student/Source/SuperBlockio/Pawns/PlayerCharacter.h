// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EDirectionEnum : uint8
{
    DE_Right 	UMETA(DisplayName = "Right"),
    DE_Left 	UMETA(DisplayName = "Left")
};

UCLASS()
class SUPERBLOCKIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


/* Custom Components and Vairables*/
    // declare overlap begin function
    UFUNCTION()
        void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
        float FireTimerDelay = 3;

    FTimerHandle SetCanShootTimer;

    UFUNCTION()
        void SetCanShoot();

    bool bCanShoot = false;

    void HasDied();


private:
    //Gravity when Jumping is not pressed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
        float EndJumpGravity = 20.0f;

    //Direction one is Facing
    EDirectionEnum DirectionEnum;

protected:

    //Visuals, CapsuleComponent otherwise is default thanks to Character
        //Visuals, CapsuleComponent otherwise is default thanks to Character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
        class UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
        class UPaperSpriteComponent* PlayerSprite;

    //Camera Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* SpringArmComponent;

    //Reference to the Class so we can set what kind of Projectile possibly
    UPROPERTY(EditAnywhere, noclear, BlueprintReadOnly, Category = Classes)
        TSubclassOf<class ABounceProjectile> Projectile;

    //Timeline
    UPROPERTY()
        class UTimelineComponent* MyTimeline;

    UPROPERTY(EditDefaultsOnly)
        UCurveFloat* FloatCurve;

    UFUNCTION()
        void TimelineCallback(float val);

    UFUNCTION()
        void TimelineFinishedCallback();

    void PlayTimeline();

    UPROPERTY()
        TEnumAsByte<ETimelineDirection::Type> Gravity;

    FOnTimelineFloat onTimelineCallback;
    FOnTimelineEventStatic onTimelineFinishedCallback;

    //Functions for Movement
    virtual void JumpPressed();
    virtual void JumpReleased();
    void MoveRight(float value);

    //Shoot that Projectile Booi
    virtual void Shoot();

    //Movement Variables
    float TravelDirection;

    //If it landed....
    void Landed(const FHitResult& Hit) override;

};
