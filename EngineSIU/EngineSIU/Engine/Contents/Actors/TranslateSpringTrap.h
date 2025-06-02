#pragma once
#include "SpringTrap.h"


class ATranslateSpringTrap : public ASpringTrap
{
    DECLARE_CLASS(ATranslateSpringTrap, ASpringTrap)

public:
    ATranslateSpringTrap();
    virtual ~ATranslateSpringTrap() override = default;
    virtual void UpdateState(float DeltaTime) override;

    UPROPERTY(EditAnywhere, FVector, Target, = FVector::ZeroVector)

    FVector Origin;
};
