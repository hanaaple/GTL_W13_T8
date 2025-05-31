#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class UMaterial : public UObject
{
    DECLARE_CLASS(UMaterial, UObject)

public:
    UMaterial() = default;

    //virtual UObject* Duplicate(UObject* InOuter) override;

    FMaterialInfo& GetMaterialInfo() { return MaterialInfo; }
    void SetMaterialInfo(const FMaterialInfo& Info) { MaterialInfo = Info; }

    // 색상 및 재질 속성 설정자
    void SetDiffuse(const FVector& InDiffuse) { MaterialInfo.DiffuseColor = InDiffuse; }
    void SetSpecular(const FVector& InSpecular) { MaterialInfo.SpecularColor = InSpecular; }
    void SetAmbient(const FVector& InAmbient) { MaterialInfo.AmbientColor = InAmbient; }
    void SetEmissive(const FVector& InEmissive) { MaterialInfo.EmissiveColor = InEmissive; }

    // 스칼라 속성 설정자
    void SetShininess(float InShininess) { MaterialInfo.Shininess = InShininess; }
    void SetOpticalDensity(float InDensity) { MaterialInfo.IOR = InDensity; }
    void SetTransparency(float InTransparency)
    {
        MaterialInfo.Transparency = InTransparency;
        MaterialInfo.bTransparent = (InTransparency > 0.f);
    }

    void SetMetallic(float InMetallic) { MaterialInfo.Metallic = InMetallic; }
    void SetRoughness(float InRoughness) { MaterialInfo.Roughness = InRoughness; }

    void SetTextureInfo(const TArray<FTextureInfo>& InTextureInfo) { MaterialInfo.TextureInfos = InTextureInfo; }
    
    virtual void SerializeAsset(FArchive& Ar) override;
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

private:
    FMaterialInfo MaterialInfo;
};
