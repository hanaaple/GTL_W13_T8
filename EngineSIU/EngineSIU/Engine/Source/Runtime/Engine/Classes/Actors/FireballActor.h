#pragma once
#include "Components/StaticMeshComponent.h"
#include"GameFramework/Actor.h"

class UProjectileMovementComponent;
class UPointLightComponent;
class USphereComp;

class AFireballActor : public AActor
{
    DECLARE_CLASS(AFireballActor, AActor)
public:
    AFireballActor();
    virtual ~AFireballActor();
    virtual void BeginPlay() override;

protected:
    
    UPROPERTY
    (UProjectileMovementComponent*, ProjectileMovementComponent, = nullptr);
   
    UPROPERTY
    (UPointLightComponent*, PointLightComponent, = nullptr);
   
    UPROPERTY
    (UStaticMeshComponent*, SphereComp, = nullptr);

};
