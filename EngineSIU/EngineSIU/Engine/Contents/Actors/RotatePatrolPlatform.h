#pragma once
#include "GameFramework/Actor.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"


class ARotatePatrolPlatform : public AActor
{
    DECLARE_CLASS(ARotatePatrolPlatform, AActor)

public:
    ARotatePatrolPlatform();
    virtual ~ARotatePatrolPlatform() override = default;
    
    virtual void Tick(float DeltaTime) override;
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    
    UPROPERTY(EditAnywhere, float, AngularSpeed, = 1.f)

    UPROPERTY(EditAnywhere, FRotator, TargetA, = FRotator(0, 0, 0))
    UPROPERTY(EditAnywhere, FRotator, TargetB, = FRotator(30, 30, 30))

    FRotator* TargetRot = nullptr;    
};


