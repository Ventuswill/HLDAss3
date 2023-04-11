// Fill out your copyright notice in the Description page of Project Settings.


#include "LockAxisfor2D.h"
#include "Components/PrimitiveComponent.h"

ULockAxisfor2D::ULockAxisfor2D()
{

}

void ULockAxisfor2D::LockPhysicsTo2DAxis(UPrimitiveComponent* primitive)
{
    primitive->GetBodyInstance()->bLockRotation = true;// BodyInstance.bLockXRotation = true; SetConstraintMode(EDOFMode::XZPlane)
    primitive->GetBodyInstance()->bLockXRotation = true;
    primitive->GetBodyInstance()->bLockYRotation = true;
    primitive->GetBodyInstance()->bLockZRotation = true;
    primitive->GetBodyInstance()->bLockYTranslation = true;
}
