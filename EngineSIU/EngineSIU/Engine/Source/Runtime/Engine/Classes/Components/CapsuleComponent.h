
#pragma once
#include "ShapeComponent.h"

class UCapsuleComponent : public UShapeComponent
{
    DECLARE_CLASS(UCapsuleComponent, UShapeComponent)

public:
    UCapsuleComponent();

    virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;

    float GetHalfHeight() const { return CapsuleHalfHeight; }
    void SetHalfHeight(float InHeight)
    {
        InHeight = FMath::Clamp(InHeight, CapsuleRadius, 10000.f);
        CapsuleHalfHeight = InHeight;
    }

    float GetRadius() const { return CapsuleRadius; }
    void SetRadius(float InRadius)
    {
        InRadius = FMath::Clamp(InRadius, 0.f, CapsuleHalfHeight);
        CapsuleRadius = InRadius;
    }

    void GetEndPoints(FVector& OutStart, FVector& OutEnd) const;
    
private:
    UPROPERTY(
        EditAnywhere, { .Category = "Capsule" },
        float, CapsuleHalfHeight, = 0.88f;
    )

    UPROPERTY(
        EditAnywhere, { .Category = "Capsule" },
        float, CapsuleRadius, = 0.34f;
    )
};
