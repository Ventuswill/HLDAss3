// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "Actor/BounceProjectile.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"


#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AAICharacter::AAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    /*Remove the Player is special case, it wont die when interacting with anything The player is responsible for death*/
	//CALL DestroyComponent() on the BoxComponent
    BoxComponent->DestroyComponent();
    

	//CREATE the PawnSensingComponent
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    
	//CALL PawnSensingComponent->OnSeePawn.AddDynamic() passing in this, &AAICharacter::OnPawnSeen
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::OnPawnSeen);
    
	//CALL PawnSensingComponent->OnHearNoise.AddDynamic() passing in this, &AAICharacter::OnNoiseHeard
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAICharacter::OnNoiseHeard);

	//CALL Empty() on Tags
    Tags.Empty();
    
	//CALL Add() on Tags passing in "Enemy"
    Tags.Add("Enemy");
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /* Patrol Checks */
	//IF CurrentPatrolPoint NOT NULL
    if (CurrentPatrolPoint)
    {
    	//DECLARE a variable called Delta of type FVector and SET it to GetActorLocation() - CurrentPatrolPoint->GetActorLocation()
        FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
    
    	//DECLARE a variable called DistanceToGoal of type float and SET it to the return value of Delta.Size()
        float DistanceToGoal = Delta.Size();

        /* Check if we are within 50 .. 100(Better with 2D) units of our goal, if so - pick a new patrol point*/
    	//IF DistanceToGoal LESS THAN 100
        if (DistanceToGoal < 100)
        {
        	//CALL MoveToNextPatrolPoint()
            MoveToNextPatrolPoint();
        }
    	//ENDIF

    	//CALL SetActorLocation() passing in FMath::VInterpConstantTo(GetActorLocation(), CurrentPatrolPoint->GetActorLocation(), DeltaTime, 600.f)
        SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), CurrentPatrolPoint->GetActorLocation(), DeltaTime, 600.f));
    }
	//ENDIF
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
    Super::BeginPlay();

	//IF bPatrol IS true
    if (bPatrol)
    {
    	//CALL MoveToNextPatrolPoint()
        MoveToNextPatrolPoint();
    
    }
	//ENDIF

	//CALL GetWorldTimerManager().SetTimer() passing in SpawnProjectileTimer, this, &AAICharacter::Shoot, FireTimerDelay, true
    GetWorldTimerManager().SetTimer(SpawnProjectileTimer, this, &AAICharacter::Shoot, FireTimerDelay, true);
}

void AAICharacter::OnPawnSeen(APawn* SeenPawn)
{
    
    if (SeenPawn == nullptr)
    {
        return;
    }

    //SET TargetActor to the SeenPawn (This can now be used to Implement a Chase behavior)
    TargetActor = SeenPawn;

    //CALL DrawDebugSphere() passing in (GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f)
    DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

}


void AAICharacter::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
    //CALL DrawDebugSphere() passing in (GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f)
    DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);


    //DECLARE a variable called Direction of type FVector and Assign it to the result of Location -  GetActorLocation()
    FVector Direction = Location - GetActorLocation();

    //CALL Normalize() on Direction
    Direction.Normalize();


    //DECLARE a variable called NewLookAt of type FRotator and SET it to the return value of FRotationMatrix::MakeFromX(Direction).Rotator()
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();

    //SET NewLookAt.Pitch to 0.0f
    NewLookAt.Pitch = 0.0f;

    //SET NewLookAt.Roll to 0.0f
    NewLookAt.Roll = 0.0f;

    //CALL SetActorRotation() passing in NewLookAt
    SetActorRotation(NewLookAt);


}

void AAICharacter::Shoot()
{
    //IF Projectile NOT NULL
    if (Projectile)
    {
        //DECLARE a variable called World of type UWorld* const and assign it to the return value of GetWorld()
        UWorld* const World = GetWorld();
        //IF World IS NOT NULL
        if (World)
        {
        	//IF TargetActor NOT NULL
            if (TargetActor)
            {
                /*Spawn Params for our Ball and AI*/
                //DECLARE a variable called SpawnParams of type FActorSpawnParameters
                FActorSpawnParameters SpawnParams;
                //SET SpawnParams.Owner to this
                SpawnParams.Owner = this;
                //SET SpawnParams.Instigator to the return value of GetInstigator()
                SpawnParams.Instigator = GetInstigator();
                //SET SpawnParams.SpawnCollisionHandlingOverride to ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                //DECLARE a variable called SpawnLocation of type FVector and assign it to the return value of GetCapsuleComponent()->GetComponentLocation()
                FVector SpawnLocation = GetCapsuleComponent()->GetComponentLocation();

                /*AI Fires balls at the player*/
                //DECLARE a variable called BounceProjectile of type ABounceProjectile* and assign it to the return value of Cast<ABounceProjectile>(GetWorld()->SpawnActor<ABounceProjectile>(Projectile, SpawnLocation, FRotator(0, 0, 0), SpawnParams))
                ABounceProjectile* BounceProjectile = Cast<ABounceProjectile>(GetWorld()->SpawnActor<ABounceProjectile>(Projectile, SpawnLocation, FRotator(0, 0, 0), SpawnParams));

            	//DECLARE a variable called Direction of type FVector and assign it to the return value of TargetActor->GetActorLocation()
                FVector Direction = TargetActor->GetActorLocation();
            	//SET Direction to Direction - SpawnLocation
                Direction = Direction - SpawnLocation;

               	//DECLARE a variable called Length of type float and assign it to the return value of Direction.Size()
                float Length = Direction.Size();
            	//IF Length >= 1
                if (Length >= 1)
                {
                	//DIVIDE Direction.X by Length i.e Direction.X /= Length
                    Direction.X /= Length;
    
                	//DIVIDE Direction.Y by Length
                    Direction.Y /= Length;
    
                	//DIVIDE Direction.Z by Length
                    Direction.Z /= Length;
    
                }
            	//ENDIF

            	//CALL BounceProjectile->ShootThatProjectile() passing in Direction
                BounceProjectile->ShootThatProjectile(Direction);
    

            	//SET TargetActor to nullptr
                TargetActor = nullptr;
    
            }
        	//ENDIF
        }
    	//ENDIF
    }
	//ENDIF
}

void AAICharacter::MoveToNextPatrolPoint()
{
    /*Assign next patrol point.*/
    //IF CurrentPatrolPoint IS nullptr OR CurrentPatrolPoint IS SecondPatrolPoint
    if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
    {
    	//SET CurrentPatrolPoint to FirstPatrolPoint
        CurrentPatrolPoint = FirstPatrolPoint;
    
    }
	//ELSE
    else
    {
    	//SET CurrentPatrolPoint to SecondPatrolPoint
        CurrentPatrolPoint = SecondPatrolPoint;
    }
	//ENDIF

	//DECLARE a variable called direction of type FVector and assign it to --> GetActorLocation() - CurrentPatrolPoint->GetActorLocation()
    FVector Direction = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();

    
    //DECLARE a variable called Length of type float and assign it to the return value of Direction.Size()
    float Length = Direction.Size();
	//IF Length GREATER THAN OR EQUAL to 1
    if (Length >= 1)
    {
        //DIVIDE Direction.X by Length and Assign it back to Direction.X 
        Direction.X /= Length;
    
        //DIVIDE Direction.Y by Length and Assign it back to Direction.Y
        Direction.Y /= Length;

    
        //DIVIDE Direction.Z by Length and Assign it back to Direction.Z
        Direction.Z /= Length;
    }
	//ENDIF

	//DECLARE a variable called NewLookAt of type FRotator and assign it to the return value of FRotationMatrix::MakeFromX(Direction).Rotator()
    FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	//SET NewLookAt.Pitch to 0.0f
    NewLookAt.Pitch = 0.f;
	//SET NewLookAt.Roll to 0.0f
    NewLookAt.Roll = 0.f;
	//INCREMENT NewLookAt.Yaw by -180.0f (+= -180.0f)
    NewLookAt.Yaw += -180.f;
	//CALL SetActorRotation() passing in NewLookAt
    SetActorRotation(NewLookAt);
}