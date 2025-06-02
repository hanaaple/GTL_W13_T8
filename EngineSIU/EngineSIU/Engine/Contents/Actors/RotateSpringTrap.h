#pragma once
#include "SpringTrap.h"


class ARotateSpringTrap : public ASpringTrap
{
    DECLARE_CLASS(ARotateSpringTrap, ASpringTrap)

public:
    ARotateSpringTrap() = default;
    virtual ~ARotateSpringTrap() override = default;

    virtual void BeginPlay() override;

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    
    virtual void UpdateState(float DeltaTime) override;

    UPROPERTY(EditAnywhere, FRotator, Target, = FRotator(0, 0, 0))

    FRotator Origin;
};
