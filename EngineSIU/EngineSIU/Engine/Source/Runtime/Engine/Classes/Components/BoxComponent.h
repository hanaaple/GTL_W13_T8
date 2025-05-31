#pragma once
#include "ShapeComponent.h"

class UBoxComponent : public UShapeComponent
{
    DECLARE_CLASS(UBoxComponent, UShapeComponent)

public:
    UBoxComponent();

    //virtual UObject* Duplicate(UObject* InOuter) override;
    
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

    FVector GetBoxExtent() const { return BoxExtent; }
    void SetBoxExtent(FVector InExtent) { BoxExtent = InExtent; }

private:
    UPROPERTY(
        EditAnywhere, { .Category = "Box"},
        FVector, BoxExtent, = FVector::OneVector;
    )
};
