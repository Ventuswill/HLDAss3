// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ONOFFBlockComponent.generated.h"

UENUM(BlueprintType)
enum class EBlockOnOff : uint8
{
    Trigger UMETA(DisplayName = "Trigger Block"),
    OnOff UMETA(DisplayName = "OnOff Block"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERBLOCKIO_API UONOFFBlockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UONOFFBlockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void SwitchStates(class AONOFFBlock* block);
    bool GetState();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ONOFF")
        bool State;

		
};
