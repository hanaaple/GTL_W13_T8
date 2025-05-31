#pragma once
#include "PrimitiveComponent.h"
#include <Math/Color.h>

class UHeightFogComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UHeightFogComponent, UPrimitiveComponent)
private:
    UPROPERTY(EditAnywhere, float, FogDensity, =  0.5f)
    UPROPERTY(EditAnywhere, float, FogHeightFalloff, = 0.05f)
    UPROPERTY(EditAnywhere, float, StartDistance, = 0.0f)
    UPROPERTY(EditAnywhere, float, FogDistanceWeight, = 0.1f)
    UPROPERTY(EditAnywhere, float, EndDistance, = 0.75f)
    UPROPERTY(EditAnywhere, FLinearColor, FogInscatteringColor, = FLinearColor::White)

public:
    UHeightFogComponent(float Density = 0.5f, float HeightFalloff = 0.05f, float StartDist = 0.f, float EndDist = 0.1f, float DistanceWeight = 0.75f);

    float GetFogDensity() const { return FogDensity; }
    float GetFogHeightFalloff() const { return FogHeightFalloff; }
    float GetStartDistance() const { return StartDistance; }
    float GetFogDistanceWeight() const { return FogDistanceWeight; }
    float GetEndDistance() const { return EndDistance; }
    FLinearColor GetFogColor() const { return FogInscatteringColor; }
    
    void SetFogDensity(float Value);
    void SetFogHeightFalloff(float Value);
    void SetStartDistance(float Value);
    void SetFogDistanceWeight(float Value);
    void SetEndDistance(float Value);
    void SetFogColor(FLinearColor Color);

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    
};
