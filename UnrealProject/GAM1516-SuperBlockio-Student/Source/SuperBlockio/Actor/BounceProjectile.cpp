// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceProjectile.h"

#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Game/SuperBlockioGameStateBase.h"

#include "Pawns/PlayerCharacter.h"

#include "Helpers/LockAxisfor2D.h"

// Sets default values
ABounceProjectile::ABounceProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CREATE the SphereComponent
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//CALL ULockAxisfor2D::LockPhysicsTo2DAxis() passing in SphereComponent
    ULockAxisfor2D::LockPhysicsTo2DAxis(SphereComponent);
    

	//CALL SphereComponent->OnComponentBeginOverlap.AddDynamic() passing in this, &ABounceProjectile::OnOverlapBegin
    SphereComponent->OnComponentHit.AddDynamic(this, &ABounceProjectile::OnHit);
    

	//SET the SphereComponent as the RootComponent 
    RootComponent = SphereComponent;
    

	//CREATE the BallSprite Component
    BallSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BallSprite"));
    
	//CALL SetCollisionProfileName() on BallSprite passing in "NoCollision"
    BallSprite->SetCollisionProfileName("NoCollision");
    
	//CALL SetCollisionEnabled() on BallSprite passing in ECollisionEnabled::NoCollision
    BallSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	//CALL SetupAttachment() on BallSprite passing in RootComponent
    BallSprite->SetupAttachment(RootComponent);
    

    /** ProjectileMovementComponent  */
	//CREATE the ProjectileMovementComponent
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    
	//SET the ProjectileMovementComponent->UpdatedComponent to SphereComponent
    ProjectileMovementComponent->UpdatedComponent = SphereComponent;
    

	//SET ProjectileMovementComponent->bRotationFollowsVelocity to false
    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    
	//SET ProjectileMovementComponent->bShouldBounce to true
    ProjectileMovementComponent->bShouldBounce = true;
    
	//SET ProjectileMovementComponent->ProjectileGravityScale to 0
    ProjectileMovementComponent->ProjectileGravityScale = 0;
    
	//SET ProjectileMovementComponent->Bounciness to 1.0
    ProjectileMovementComponent->Bounciness = 1.0;

    //CALL Add() on Tags passing in "Ball"
    Tags.Add("Ball");

	//SET InitialLifeSpan to 5
    InitialLifeSpan = 5;

	//CALL ProjectileMovementComponent->OnProjectileBounce.AddDynamic() passing in this, &ABounceProjectile::OnProjectileBounce
    ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ABounceProjectile::OnProjectileBounce);
}

// Called when the game starts or when spawned
void ABounceProjectile::BeginPlay()
{
	Super::BeginPlay();
	//SET ProjectileMovementComponent->InitialSpeed to Speed
    ProjectileMovementComponent->InitialSpeed = Speed;
	//CALL ProjectileMovementComponent->MaxSpeed to Speed
    ProjectileMovementComponent->MaxSpeed = Speed;


    FVector newLocation;

    newLocation = GetActorLocation();

    newLocation.Y = 0;

    RootComponent->SetWorldLocation(newLocation);
    
}

// Called every frame
void ABounceProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector newLocation;

    newLocation = GetActorLocation();

    newLocation.Y = 0;

    RootComponent->SetWorldLocation(newLocation);
}

void ABounceProjectile::ShootThatProjectile(const FVector& Direction)
{
	//SET LaunchDir to Direction
    LaunchDir = Direction;
    
	//SET ProjectileMovementComponent->Velocity to LaunchDir * Speed
    ProjectileMovementComponent->Velocity = LaunchDir * Speed;
}

void ABounceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (OtherActor)
    {
        /*If it hits against Enemy/Player, else if it hits against world stuff*/
        //IF OtherActor NOT EQUAL to this->GetOwner()
        if (OtherActor != this->GetOwner())
        {
            //IF OtherActor->ActorHasTag("Enemy")
            if (OtherActor->ActorHasTag("Enemy"))
            {
                //CALL Destroy() on OtherActor
                OtherActor->Destroy();

                //CAll Destroy() on this
                this->Destroy();


                //DECLARE a variable called gamestate of type ASuperBlockioGameStateBase and assign it to the return of GetWorld()->GetGameState<ASuperBlockioGameStateBase>()
                ASuperBlockioGameStateBase* gamestate = GetWorld()->GetGameState<ASuperBlockioGameStateBase>();

                //IF gamestate NOT nullptr
                if (gamestate)
                {
                    //CALL IncreaseScore() on gamestate passing in 500
                    gamestate->IncreaseScore(500);
                }
                //ENDIF
            }
            //ELSE IF OtherActor->ActorHasTag("Player")
            else if (OtherActor->ActorHasTag("Player"))
            {
                //DECLARE a variable called player of type APlayerCharacter* and assign it to the return value of Cast<APlayerCharacter>(OtherActor)
                APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

                //CALL HasDied() on player
                player->HasDied();

                //CALL Destroy() on this
                this->Destroy();

            }
            // ELSE IF OtherActor->ActorHasTag("Coin")
            else if (OtherActor->ActorHasTag("Coin"))
            {
                //IF this->GetOwner()->ActorHasTag("Player")
                if (this->GetOwner()->ActorHasTag("Player"))
                {
                    //DECLARE a variable called gamestate of type ASuperBlockioGameStateBase* and assign it to the return value of GetWorld()->GetGameState<ASuperBlockioGameStateBase>()
                    ASuperBlockioGameStateBase* gamestate = GetWorld()->GetGameState<ASuperBlockioGameStateBase>();

                    //IF  gamestate NOT nullptr
                    if (gamestate)
                    {
                        //CALL IncreaseCoins() on gamestate passing in 1
                        gamestate->IncreaseCoins(1);
                        //CALL IncreaseScore() on gamestate passing in 100
                        gamestate->IncreaseScore(100);
                    }//ENDIF

                    //CALL Destroy() on OtherActor
                    OtherActor->Destroy();

                }
                //ENDIF
            }//ENDIF
        }
        //ENDIF
    }
}

void ABounceProjectile::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("OnBounce"));

	//DECLARE a variable called ImpactNormal of type FVector and SET it to ImpactResult.ImpactNormal
    FVector ImpactNormal = ImpactResult.ImpactNormal;

	//DECLARE a variable called Velocity of type FVector and SET it to ProjectileMovementComponent->Velocity
    FVector Velocity = ProjectileMovementComponent->Velocity;


	//DECLARE a variable called Length of type float and SET it to the return value of Velocity.Size()
    float Length = Velocity.Size();

	//IF Length GREATER THAN OR EQUAL to 1
    if (Length >= 1)
    {
    	//DIVIDE Velocity.X by Length i.e Velocity.X /= Length;
        Velocity.X /= Length;

    	//DIVIDE Velocity.Y by Length
        Velocity.Y /= Length;

    	//DIVIDE Velocity.Z by Length
        Velocity.Z /= Length;

    }

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Velocity: X -") + FString::SanitizeFloat(Velocity.X) + " Y - " + FString::SanitizeFloat(Velocity.Y) + " Z - " + FString::SanitizeFloat(Velocity.Z));

	//CALL Velocity.MirrorByVector() passing in ImpactNormal
    Velocity.MirrorByVector(ImpactNormal);

	//SET ProjectileMovementComponent->Velocity to Velocity * Speed
    ProjectileMovementComponent->Velocity = Velocity * Speed;
}

