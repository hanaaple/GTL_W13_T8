#pragma once
#include "Components/MeshComponent.h"
#include "Engine/StaticMesh.h"

#include "Engine/Asset/StaticMeshAsset.h"

class UStaticMeshComponent : public UMeshComponent
{
    DECLARE_CLASS(UStaticMeshComponent, UMeshComponent)

public:
    UStaticMeshComponent() = default;


    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;

    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

    void SetselectedSubMeshIndex(const int& Value) { SelectedSubMeshIndex = Value; }
    int GetselectedSubMeshIndex() const { return SelectedSubMeshIndex; }
    
    virtual uint32 GetNumMaterials() const override;
    virtual UMaterial* GetMaterial(uint32 ElementIndex) const override;
    virtual uint32 GetMaterialIndex(FName MaterialSlotName) const override;
    virtual TArray<FName> GetMaterialSlotNames() const override;
    virtual void GetUsedMaterials(TArray<UMaterial*>& Out) const override;

    virtual int CheckRayIntersection(const FVector& InRayOrigin, const FVector& InRayDirection, float& OutHitDistance) const override;
    
    UStaticMesh* GetStaticMesh() const { return StaticMesh; }
    void SetStaticMesh(UStaticMesh* Value)
    { 
        StaticMesh = Value;
        if (StaticMesh == nullptr)
        {
            OverrideMaterials.SetNum(0);
            AABB = FBoundingBox(FVector::ZeroVector, FVector::ZeroVector);
        }
        else
        {
            OverrideMaterials.SetNum(Value->GetMaterials().Num());
            AABB = FBoundingBox(StaticMesh->GetRenderData()->BoundingBoxMin, StaticMesh->GetRenderData()->BoundingBoxMax);
        }
    }

protected:
    UPROPERTY(EditAnywhere | ShallowCopy, UStaticMesh*, StaticMesh, = nullptr)
    //UStaticMesh* StaticMesh = nullptr;
    int SelectedSubMeshIndex = -1;
};
