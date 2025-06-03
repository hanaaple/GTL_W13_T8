#pragma once
#include <algorithm>

#include "Math/Vector.h"
#include "UObject/ObjectMacros.h"

struct FDistributionVector
{
    DECLARE_STRUCT(FDistributionVector)

    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, MinValue, = FVector::ZeroVector)
    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, MaxValue, = FVector::ZeroVector)

    //FVector MinValue;
    //FVector MaxValue;

    std::mt19937 Gen;
    std::uniform_real_distribution<float> DistX;
    std::uniform_real_distribution<float> DistY;
    std::uniform_real_distribution<float> DistZ;

public:
    FDistributionVector()
        : MinValue(FVector::ZeroVector)
        , MaxValue(FVector::ZeroVector)
        , Gen(std::random_device{}())
        , DistX(MinValue.X, MaxValue.X)
        , DistY(MinValue.Y, MaxValue.Y)
        , DistZ(MinValue.Z, MaxValue.Z)
    {}

    // 범위를 지정하는 생성자
    FDistributionVector(const FVector& InMin, const FVector& InMax)
        : MinValue(InMin)
        , MaxValue(InMax)
        , Gen(std::random_device{}())
        , DistX(MinValue.X, MaxValue.X)
        , DistY(MinValue.Y, MaxValue.Y)
        , DistZ(MinValue.Z, MaxValue.Z)
    {   }

    void GetOutRange(FVector& MinOut, FVector& MaxOut);

    void UpdateDistributionParam()
    {
        MaxValue.X = FMath::Max(MinValue.X, MaxValue.X);
        MaxValue.Y = FMath::Max(MinValue.Y, MaxValue.Y);
        MaxValue.Z = FMath::Max(MinValue.Z, MaxValue.Z);

        DistX = std::uniform_real_distribution<float>(MinValue.X, MaxValue.X);
        DistY = std::uniform_real_distribution<float>(MinValue.Y, MaxValue.Y);
        DistZ = std::uniform_real_distribution<float>(MinValue.Z, MaxValue.Z);
    }

    void UpdateRange(const FVector& InMin, const FVector& InMax)
    {
        MinValue = InMin;
        MaxValue = InMax;
        
        UpdateDistributionParam();
    }

    FVector GetValue()
    {
        UpdateDistributionParam();

        return FVector(DistX(Gen), DistY(Gen), DistZ(Gen));
    }

    friend FArchive& operator<<(FArchive& Ar, FDistributionVector& DV)
    {
        Ar << DV.MinValue << DV.MaxValue;

        if (Ar.IsLoading())
        {
            DV.UpdateDistributionParam();
        }

        return Ar;
    }
};
