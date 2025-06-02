#pragma once
#include "Engine/StaticMeshActor.h"


class ATranslatePatrolPlatform : public AActor
{
    DECLARE_CLASS(ATranslatePatrolPlatform, AActor)

public:
    ATranslatePatrolPlatform();
    virtual ~ATranslatePatrolPlatform() override = default;

    virtual void Tick(float DeltaTime) override;
    
    UPROPERTY(EditAnywhere, float, Speed, = 1.f)

    UPROPERTY(EditAnywhere, FVector, TargetA, = FVector(0, 0, 0))
    UPROPERTY(EditAnywhere, FVector, TargetB, = FVector(1, 1, 1))

    FVector* TargetPos = nullptr;
};
