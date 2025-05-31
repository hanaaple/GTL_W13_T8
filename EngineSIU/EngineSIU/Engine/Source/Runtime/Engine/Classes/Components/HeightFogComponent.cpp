#include "HeightFogComponent.h"
#include <UObject/Casts.h>

UHeightFogComponent::UHeightFogComponent(float Density, float HeightFalloff, float StartDist, float EndDist, float DistanceWeight)
    :FogDensity(Density), FogHeightFalloff(HeightFalloff), StartDistance(StartDist), FogDistanceWeight(DistanceWeight), EndDistance(EndDist)
{
    FogInscatteringColor = FLinearColor::White;
}

void UHeightFogComponent::SetFogDensity(float Value)
{
    FogDensity = Value;
}

void UHeightFogComponent::SetFogHeightFalloff(float Value)
{
    FogHeightFalloff = Value; 
}

void UHeightFogComponent::SetStartDistance(float Value)
{
    StartDistance = Value;
}

void UHeightFogComponent::SetFogDistanceWeight(float Value)
{
    FogDistanceWeight = Value;
}

void UHeightFogComponent::SetEndDistance(float Value)
{
    EndDistance = Value;
}

void UHeightFogComponent::SetFogColor(FLinearColor Color)
{
    FogInscatteringColor = Color;
}

void UHeightFogComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
    OutProperties.Add(TEXT("FogDensity"), FString::Printf(TEXT("%f"), FogDensity));
    OutProperties.Add(TEXT("FogHeightFalloff"), FString::Printf(TEXT("%f"), FogHeightFalloff));
    OutProperties.Add(TEXT("StartDistance"), FString::Printf(TEXT("%f"), StartDistance));
    OutProperties.Add(TEXT("FogCutoffDistance"), FString::Printf(TEXT("%f"), FogDistanceWeight));
    OutProperties.Add(TEXT("FogMaxOpacity"), FString::Printf(TEXT("%f"), EndDistance));
    //FVector4 Color = FVector4(FogInscatteringColor.R, FogInscatteringColor.G, FogInscatteringColor.B, FogInscatteringColor.A);
    
    OutProperties.Add(TEXT("FogInscatteringColor"), FString::Printf(TEXT("%s"), *FogInscatteringColor.ToString()));
}

void UHeightFogComponent::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("FogDensity"));
    if (TempStr)
    {
        FogDensity = FString::ToFloat(*TempStr);
    }
    TempStr = InProperties.Find(TEXT("FogHeightFalloff"));
    if (TempStr)
    {
        FogHeightFalloff = FString::ToFloat(*TempStr);
    }
    TempStr = InProperties.Find(TEXT("StartDistance"));
    if (TempStr)
    {
        StartDistance = FString::ToFloat(*TempStr);
    }
    TempStr = InProperties.Find(TEXT("FogCutoffDistance"));
    if (TempStr)
    {
        FogDistanceWeight = FString::ToFloat(*TempStr);
    }
    TempStr = InProperties.Find(TEXT("FogMaxOpacity"));
    if (TempStr)
    {
        EndDistance = FString::ToFloat(*TempStr);
    }
    TempStr = InProperties.Find(TEXT("FogInscatteringColor"));
    if (TempStr)
    {
        // FVector4 Color;
        // Color.InitFromString(*TempStr);
        FogInscatteringColor = FLinearColor(*TempStr);
    }
}
