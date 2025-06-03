#pragma once
#include "GameFramework/Actor.h"


class AFish : public AActor
{
    DECLARE_CLASS(AFish, AActor)

public:
    AFish();

    virtual ~AFish() override = default;
    
    virtual void PostSpawnInitialize() override;
    virtual void BeginPlay() override;

    UPROPERTY
    (class UStaticMeshComponent*, StaticMeshComponent, = nullptr);

    UPROPERTY
    (class USkeletalMeshComponent*, SkeletalMeshComponent, = nullptr);
};
