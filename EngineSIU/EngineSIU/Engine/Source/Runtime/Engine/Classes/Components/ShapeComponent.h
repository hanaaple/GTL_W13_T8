
#pragma once
#include "PrimitiveComponent.h"

enum class EShapeType : uint8
{
    Box,
    Sphere,
    Capsule,

    Max,
};

class UShapeComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UShapeComponent, UPrimitiveComponent)    

public:
    UShapeComponent();

    virtual void TickComponent(float DeltaTime) override;
    UPROPERTY(
        EditAnywhere,
        FColor, ShapeColor, = FColor(180, 180, 180, 255);
    )

    UPROPERTY(
        EditAnywhere,
        bool, bDrawOnlyIfSelected, = true;
    )

    EShapeType GetShapeType() const { return ShapeType; }

protected:
    UPROPERTY(EditAnywhere, EShapeType, ShapeType, = EShapeType::Max)
};
