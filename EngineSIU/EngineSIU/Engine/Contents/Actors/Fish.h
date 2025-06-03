#pragma once
#include "GameFramework/Actor.h"


class AFish : public AActor
{
    DECLARE_CLASS(AFish, AActor)

public:
    AFish();

    virtual void PostSpawnInitialize() override;
    
    virtual ~AFish() override = default;

    UPROPERTY
    (class UStaticMeshComponent*, StaticMeshComponent, = nullptr);

    UPROPERTY
    (class USkeletalMeshComponent*, SkeletalMeshComponent, = nullptr);
};
