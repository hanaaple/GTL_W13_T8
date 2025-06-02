#pragma once
#include "SpringTrap.h"


class ATranslateSpringTrap : public ASpringTrap
{
    DECLARE_CLASS(ATranslateSpringTrap, ASpringTrap)

public:
    ATranslateSpringTrap() = default;
    virtual ~ATranslateSpringTrap() override = default;

    virtual void BeginPlay() override;

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    
    virtual void UpdateState(float DeltaTime) override;

    UPROPERTY(EditAnywhere, FVector, Target, = FVector::ZeroVector)

    FVector Origin;
};
