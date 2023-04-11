// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockioHUD.h"

#include "Blueprint/UserWidget.h"
#include "ConstructorHelpers.h"

ABlockioHUD::ABlockioHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> BlockioHud(TEXT("/Game/Blueprints/UI/BlockioHUD"));
    if (BlockioHud.Succeeded())
        BlockioGUIClass = BlockioHud.Class;
}

void ABlockioHUD::BeginPlay()
{
	//SET BlockioGUI to the return value of CreateWidget<UUserWidget>(GetGameInstance(), BlockioGUIClass)
    
	//CALL AddToViewport() on BlockioGUI
    
}
