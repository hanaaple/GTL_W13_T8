#pragma once
#include "UObject/ObjectMacros.h"

struct FDistributionFloat
{
    DECLARE_STRUCT(FDistributionFloat)

    FDistributionFloat() : MinValue(1), MaxValue(1) {}
    FDistributionFloat(float InMinValue, float InMaxValue) : MinValue(InMinValue), MaxValue(InMaxValue) {}

    UPROPERTY_WITH_FLAGS(EditAnywhere, float, MinValue, = 1)
    UPROPERTY_WITH_FLAGS(EditAnywhere, float, MaxValue, = 1)

    //float MinValue;
    //float MaxValue;

    void GetOutRange(float& MinOut, float& MaxOut);
    float GetValue();

    friend FArchive& operator<<(FArchive& Ar, FDistributionFloat& DF)
    {
        return Ar << DF.MinValue << DF.MaxValue;
    }
};
