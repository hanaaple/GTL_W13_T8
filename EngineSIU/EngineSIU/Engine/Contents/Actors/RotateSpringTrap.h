#pragma once
#include "SpringTrap.h"


class ARotateSpringTrap : public ASpringTrap
{
    DECLARE_CLASS(ARotateSpringTrap, ASpringTrap)

public:
    ARotateSpringTrap();
    virtual ~ARotateSpringTrap() override = default;
    virtual void UpdateState(float DeltaTime) override;

    UPROPERTY(EditAnywhere, FRotator, Target, = FRotator(0, 0, 0))

    FRotator Origin;
};
