#pragma once

#include "GameFramework/Actor.h"

class UHeightFogComponent;

class AHeightFogActor : public AActor
{
    DECLARE_CLASS(AHeightFogActor, AActor)

public:
    AHeightFogActor();

    UPROPERTY(EditAnywhere, UHeightFogComponent*, HeightFogComponent, = nullptr)
};
