// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Actor/BounceProjectile.h"

#include "Classes/PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "UObject/ConstructorHelpers.h"
#include "Helpers/LockAxisfor2D.h"
#include "Game/SuperBlockioGameModeBase.h"
#include "Game/SuperBlockioGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //We don't need this
	//CALL  GetMesh()->DestroyComponent()
    GetMesh()->DestroyComponent();

    /*Modify the Inherited Capsule Component*/
	//CALL ULockAxisfor2D::LockPhysicsTo2DAxis() passing in GetCapsuleComponent()
    ULockAxisfor2D::LockPhysicsTo2DAxis(GetCapsuleComponent());
    

	//CREATE the BoxComponent
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    
	//CALL SetCollisionProfileName() on BoxComponent passing in "OverlappAll"
    BoxComponent->SetCollisionProfileName("OverlapAll");
    
	//CALL SetupAttachment() on BoxComponent passing in RootComponent
    BoxComponent->SetupAttachment(RootComponent);
    
	//CALL BoxComponent->OnComponentBeginOverlap.AddDynamic() passing in this, &APlayerCharacter::OnOverlap
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlap);

	//CREATE the PlayerSprite Component
    PlayerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerSprite"));
    
	//CALL SetCollisionProfileName() on PlayerSprite passing in "NoCollision"
    PlayerSprite->SetCollisionProfileName("NoCollision");
    
	//CALL SetCollisionEnabled() on PlayerSprite passing in ECollisionEnabled::NoCollision
    PlayerSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CALL SetGenerateOverlapEvents() on PlayerSprite passing in false
    PlayerSprite->SetGenerateOverlapEvents(false);
	//CALL SetupAttachment() on PlayerSprite passing in RootComponent
    PlayerSprite->SetupAttachment(RootComponent);

	//CREATE the SpringArmComponent
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	//CALL SetupAttachment() on SpringArmComponent passing in RootComponent
    SpringArmComponent->SetupAttachment(RootComponent);
	//SET  SpringArmComponent->bInheritPitch to false
    SpringArmComponent->bInheritPitch = false;
	//SET SpringArmComponent->bInheritYaw to false
    SpringArmComponent->bInheritYaw = false;
	//SET SpringArmComponent->bInheritRoll to false
    SpringArmComponent->bInheritRoll = false;
	//CALL SetRelativeRotation() on SpringArmComponent passing in FRotator(0.f, -90.f, 0.f)
    SpringArmComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//CREATE the FollowCamera Component
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//CALL SetProjectionMode() on FollowCamera passing in ECameraProjectionMode::Orthographic
    FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	//CALL SetOrthoWidth() on FollowCamera passing in 5000.0f
    FollowCamera->SetOrthoWidth(5000.0f);
	//CALL SetupAttachment on FollowCamera passing in SpringArmComponent
    FollowCamera->SetupAttachment(SpringArmComponent);

    /*Timeline Curve set*/
    static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curves/C_JumpCurve"));
    check(Curve.Succeeded());

    FloatCurve = Curve.Object;

    //SET the DirectionEnum to DE_Right and add the PlayerTag
    DirectionEnum = EDirectionEnum::DE_Right;
    Tags.Add("Player");

    /*Set our Projectile to the BounceProjectile Class*/
    Projectile = ABounceProjectile::StaticClass();

    GetCharacterMovement()->MaxStepHeight = 0;
    GetCharacterMovement()->PerchRadiusThreshold = 40;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    /*Example Timeline taken from Unreal Wiki*/
	//IF FloatCurve NOT EQUAL NULL
    if (FloatCurve)
    {
    	//SET MyTimeline to the return value of NewObject<UTimelineComponent>(this, FName("Jump Gravity"))
        MyTimeline = NewObject<UTimelineComponent>(this, FName("Jump Gravity"));

        /* Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts */
    	//SET MyTimeline->CreationMethod to EComponentCreationMethod::UserConstructionScript
        MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;

    	//CALL Add() on BlueprintCreatedComponents passing in MyTimeline
        BlueprintCreatedComponents.Add(MyTimeline);
    	//CALL SetNetAddressable() on MyTimeline
        MyTimeline->SetNetAddressable();

    	//CALL SetPropertySetObject() on MyTimeline passing in this
        MyTimeline->SetPropertySetObject(this);
    	//CALL SetDirectionPropertyName() on  MyTimeline passing in FName("Gravity")
        MyTimeline->SetDirectionPropertyName(FName("Gravity"));

    	//CALL SetLooping() on MyTimeline passing in false
        MyTimeline->SetLooping(false);
    	//CALL SetTimelineLength() on MyTimeline passing in 1
        MyTimeline->SetTimelineLength(1);
    	//CALL SetPlaybackPosition() on MyTimeline passing in 0.0f, false
        MyTimeline->SetPlaybackPosition(0.0f, false);

        /*Add the float curve to the timeline and connect it to your timelines's interpolation function*/
    	//CALL BindUFunction() on onTimelineCallback passing in --> this, FName{ TEXT("TimelineCallback") }
        onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
    	//CALL BindUFunction on onTimelineFinishedCallback passing in --> this, FName{ TEXT("TimelineFinishedCallback") }
        onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
    	//CALL AddInterpFloat() on MyTimeline passing in FloatCurve, onTimelineCallback
        MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);
    	//CALL SetTimelineFinishedFunc() on MyTimeline passing in onTimelineFinishedCallback
        MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

    	//CALL RegisterComponent() on MyTimeline
        MyTimeline->RegisterComponent();
    }

	//SET bCanShoot to true
    bCanShoot = true;

	//CALL  GetWorldTimerManager().SetTimer() passing in SetCanShootTimer, this, &APlayerCharacter::SetCanShoot, FireTimerDelay, true
    GetWorldTimerManager().SetTimer(SetCanShootTimer, this, &APlayerCharacter::SetCanShoot, FireTimerDelay, true);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//IF MyTimeline NOT EQUAL NULL
    if (MyTimeline)
    {
    	//CALL TickComponent() on MyTimeline passing in DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL
        MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
    }

	//IF GetCharacterMovement()->IsFalling() IS true
    if (GetCharacterMovement()->IsFalling())
    {
    	//SET GetCharacterMovement()->GravityScale to EndJumpGravity
        GetCharacterMovement()->GravityScale = EndJumpGravity;
    }
	//ENDIF


    /*Prevent any weird movement off the Y Axis*/
	//IF GetCapsuleComponent()->GetComponentLocation().Y GREATER THAN OR EQUAL TO 10 OR GetCapsuleComponent()->GetComponentLocation().Y LESS THAN OR EQUAL TO -10
    if (GetCapsuleComponent()->GetComponentLocation().Y >= 10 || GetCapsuleComponent()->GetComponentLocation().Y <= -10)
    {
    	//DECLARE a variable called currentLocation of type FVector and assign it to the return value of GetCapsuleComponent()->GetComponentLocation()
        FVector currentLocation = GetCapsuleComponent()->GetComponentLocation();
    	//SET currentLocation.Y to 0
        currentLocation.Y = 0;
    	//CALL GetCapsuleComponent()->SetWorldLocation() passing in currentLocation
        GetCapsuleComponent()->SetWorldLocation(currentLocation);
    }
	//ENDIF
	

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//CALL PlayerInputComponent->BindAction() passing in "Jump", IE_Pressed, this, &APlayerCharacter::JumpPressed
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpPressed);
	//CALL PlayerInputComponent->BindAction() passing in "Jump", IE_Released, this, &APlayerCharacter::JumpReleased
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpReleased);
	//CALL PlayerInputComponent->BindAction() passing in "Fire", IE_Pressed, this, &APlayerCharacter::Shoot
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Shoot);

	//CALL  PlayerInputComponent->BindAxis() passing in "MoveRight", this, &APlayerCharacter::MoveRight
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

}

// This function is called for every tick in the timeline.
void APlayerCharacter::TimelineCallback(float val)
{
	//SET  GetCharacterMovement()->GravityScale to val
    GetCharacterMovement()->GravityScale = val;
}

// This function is called when the timeline finishes playing.
void APlayerCharacter::TimelineFinishedCallback()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Timeline Finished"));
}

//This is to play timeline
void APlayerCharacter::PlayTimeline()
{
	//IF MyTimeline NOT EQUAL NULL
    if (MyTimeline)
    {
    	//CALL PlayFromStart() on MyTimeline
        MyTimeline->PlayFromStart();
    }
	//ENDIF
}

void APlayerCharacter::JumpPressed()
{
	//IF MyTimeline NOT EQUAL NULL
    if (MyTimeline)
    {
        /*MyCharacterMovment Component that is inherited when using Character rather than Pawn*/
    	//CALL Jump()
        Jump();
    	//CALL PlayTimeline()
        PlayTimeline();
    }
	//ENDIF
}

void APlayerCharacter::JumpReleased()
{
	//IF MyTimeline NOT EQUAL NULL
    if (MyTimeline)
    {
    	//CALL Stop() on MyTimeline
        MyTimeline->Stop();
    	//CALL GetCharacterMovement()->GravityScale to EndJumpGravity
        GetCharacterMovement()->GravityScale = EndJumpGravity;
    }
	//ENDIF
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	//SET  GetCharacterMovement()->GravityScale to 1
    GetCharacterMovement()->GravityScale = 1;
}

void APlayerCharacter::MoveRight(float value)
{
	//IF value NOT EQUAL 0.0f
    if (value != 0.f)
    {
    	//DECLARE a variable called PlayerVelocity of type const FVector and assign it to the return value of GetVelocity()
        const FVector PlayerVelocity = GetVelocity();
    	//SET TravelDirection to -PlayerVelocity.X
        TravelDirection = -PlayerVelocity.X;

        //IF PlayerSprite NOT NULL
        if (PlayerSprite)
        {
        	//IF TravelDirection LESS THAN 0.0f
            if (TravelDirection < 0.0f)
            {
            	//CALL SetRelativeRotation() on PlayerSprite passing in FRotator(FRotator::ZeroRotator)
                PlayerSprite->SetRelativeRotation(FRotator(FRotator::ZeroRotator));
    

            	//SET DirectionEnum to EDirectionEnum::DE_Right
                DirectionEnum = EDirectionEnum::DE_Right;
    
            }
        	//ELSE IF TravelDirection GREATER THAN 0.0f
            else if (TravelDirection > 0.0f)
            {
            	//CALL SetRelativeRotation() on PlayerSprite passing in FRotator(0.f, 180.f, 0.f)
                PlayerSprite->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
    

            	//SET DirectionEnum to EDirectionEnum::DE_Left
                DirectionEnum = EDirectionEnum::DE_Left;
            }
        	//ENDIF
        }
    	//ENDIF

    	//CALL AddMovementInput() passing in FVector(1.0f, 0.0f, 0.0f), value
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), value);
    }
	//ELSE
    else
    {
    	//SET TravelDirection to 0.0f
        TravelDirection = 0.0f;
    }
	//ENDIF
}

void APlayerCharacter::Shoot()
{
	//IF Projectile NOT NULL
    if (Projectile)
    {
    	//DECLARE a variable called World of type UWorld* const and assign it to the return value of GetWorld()
        UWorld* const World = GetWorld();
    	//IF World IS NOT NULL
        if (World)
        {
        	//IF bCanShoot IS true
            if (bCanShoot)
            {
            	//SET bCanShoot to false
                bCanShoot = false;
                /*Spawn Params for our Ball and AI*/
            	//DECLARE a variable called SpawnParams of type FActorSpawnParameters
                FActorSpawnParameters SpawnParams;
    
            	//SET SpawnParams.Owner to this
                SpawnParams.Owner = this;
            	//SET SpawnParams.Instigator to the return value of GetInstigator()
                SpawnParams.Instigator = GetInstigator();
            	//SET SpawnParams.SpawnCollisionHandlingOverride to ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            	//DECLARE a variable called SpawnLocation of type FVector and assign it to the return value of PlayerSprite->GetSocketLocation("Ball")
                FVector SpawnLocation = PlayerSprite->GetSocketLocation("Ball");

                /*Our Player will always fire the ball at a 45 degree angle downwards, so no real special code is needed.*/
            	//DECLARE a variable called BounceProjectile of type ABounceProjectile* and assign it to the return value of Cast<ABounceProjectile>(GetWorld()->SpawnActor<ABounceProjectile>(Projectile, SpawnLocation, FRotator(0, 0, 0), SpawnParams))
                ABounceProjectile* BounceProjectile = Cast<ABounceProjectile>(GetWorld()->SpawnActor<ABounceProjectile>(Projectile, SpawnLocation, FRotator(0, 0, 0), SpawnParams));

            	//IF BounceProjectile NOT NULL
                if (BounceProjectile)
                {
                	//IF DirectionEnum IS EDirectionEnum::DE_Left
                    if (DirectionEnum == EDirectionEnum::DE_Left)
                    {
                    	//CALL BounceProjectile->ShootThatProjectile() passing in FVector(-0.707107, 0, -0.707107)
                    BounceProjectile->ShootThatProjectile(FVector(-0.707107, 0, -0.707107));
                    }
                	//ELSE
                    else
                    {
                        //CALL BounceProjectile->ShootThatProjectile() passing in FVector(0.707107, 0, -0.707107)
                        BounceProjectile->ShootThatProjectile(FVector(0.707107, 0, -0.707107));
                    }
                	//ENDIF
                }
            	//ENDIF
            }
        	//ENDIF
        }
    	//ENDIF
    }
}

void APlayerCharacter::HasDied()
{
	//DECLARE a variable called Gamemode of type ASuperBlockioGameModeBase* and assign it to the return value of Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode())
    ASuperBlockioGameModeBase* Gamemode = Cast<ASuperBlockioGameModeBase>(GetWorld()->GetAuthGameMode());

	//IF Gamemode NOT NULL
    if (Gamemode)
    {
    	//CALL OnDeath() on Gamemode
        Gamemode->OnDeath();
    }
	//ENDIF
}

void APlayerCharacter::SetCanShoot()
{
	//SET bCanShoot to true
    bCanShoot = true;
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//IF OtherActor->ActorHasTag("Enemy")
    if (OtherActor->ActorHasTag("Enemy"))
    {
    	//CALL HasDied()
        HasDied();
    }
	//ELSE IF OtherActor->ActorHasTag("Pickup")
    else if (OtherActor->ActorHasTag("Pickup"))
    {
    	//IF OtherActor->ActorHasTag("Coin")
        if (OtherActor->ActorHasTag("Coin"))
        {
        	//DECLARE a variable called gamestate of type ASuperBlockioGameStateBase* and assign it to the return value of GetWorld()->GetGameState<ASuperBlockioGameStateBase>()
            ASuperBlockioGameStateBase* gamestate = GetWorld()->GetGameState<ASuperBlockioGameStateBase>();

        	//IF gamestate NOT NULL
            if (gamestate)
            {
            	//CALL IncreaseCoins() on gamestate passin in 1
                gamestate->IncreaseCoins(1);
            	//CALL IncreaseScore() on gamestate passing in 100
                gamestate->IncreaseScore(100);
            }
        	//ENDIF

        	//CALL Destroy() on OtherActor
            OtherActor->Destroy();
        }
    	//ENDIF
    }
	//ENDIF
}
