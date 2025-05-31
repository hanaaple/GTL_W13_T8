#pragma once
#include "Components/StaticMeshComponent.h"


class USkySphereComponent : public UStaticMeshComponent
{
    DECLARE_CLASS(USkySphereComponent, UStaticMeshComponent)

public:
    USkySphereComponent();

    virtual void TickComponent(float DeltaTime) override;

    UPROPERTY(EditAnywhere, float, UOffset, = 0.0f)
    UPROPERTY(EditAnywhere, float, VOffset, = 0.0f)
};
