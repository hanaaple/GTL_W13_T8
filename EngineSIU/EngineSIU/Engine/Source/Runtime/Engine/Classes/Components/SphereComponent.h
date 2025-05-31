#pragma once
#include "ShapeComponent.h"

class USphereComponent : public UShapeComponent
{
    DECLARE_CLASS(USphereComponent, UShapeComponent)

public:
    USphereComponent();

    //virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;

    void SetRadius(float InRadius) { SphereRadius = InRadius; }
    float GetRadius() const { return SphereRadius; }
    
private:
    UPROPERTY(
        EditAnywhere, { .Category = "Sphere" },
        float, SphereRadius, = 1.f;
    )
};
