
#include "PhysicsAsset.h"

#include "Engine/AssetManager.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/Casts.h"
#include "Engine/Engine.h"
#include "PhysicsManager.h"
#include "ConstraintInstance.h"
#include "Container/ArrayHelper.h"

void UBodySetupCore::SerializeAsset(FArchive& Ar)
{
    Ar << BoneName;
}

void UBodySetup::SerializeAsset(FArchive& Ar)
{
    UBodySetupCore::SerializeAsset(Ar);

    Ar << GeomAttributes;
}

void UBodySetup::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);

    AggGeom.BoxElems.Empty();
    AggGeom.SphereElems.Empty();
    AggGeom.CapsuleElems.Empty();
}

bool UPhysicsAsset::SetPreviewMesh(USkeletalMesh* PreviewMesh)
{
    if (PreviewMesh)
    {
        for (int32 i = 0; i < BodySetups.Num(); ++i)
        {
            FName BodyName = BodySetups[i]->BoneName;
            int32 BoneIndex = PreviewMesh->GetRefSkeleton().FindBoneIndex(BodyName);
            if (BoneIndex == INDEX_NONE)
            {
                return false;
            }
        }

        PreviewSkeletalMesh = PreviewMesh;
        return true;
    }
    return false;
}

USkeletalMesh* UPhysicsAsset::GetPreviewMesh() const
{
    return PreviewSkeletalMesh;
}

void UPhysicsAsset::SerializeAsset(FArchive& Ar)
{
    FName PreviewMeshName = NAME_None;
    if (Ar.IsSaving())
    {
        PreviewMeshName = UAssetManager::Get().GetAssetKeyByObject(EAssetType::SkeletalMesh, PreviewSkeletalMesh);
    }

    Ar << PreviewMeshName;

    if (Ar.IsLoading())
    {
        if (UObject* Asset = UAssetManager::Get().GetAsset(EAssetType::SkeletalMesh, PreviewMeshName))
        {
            PreviewSkeletalMesh = Cast<USkeletalMesh>(Asset);
        }
    }

    FArrayHelper::SerializePtrAsset(Ar, BodySetups);
    FArrayHelper::SerializePtrAsset(Ar, ConstraintSetups);
}

FArchive& operator<<(FArchive& Ar, AggregateGeomAttributes& Attributes)
{
    uint8 Type = static_cast<uint8>(Attributes.GeomType);
    
    Ar << Type;
    
    if (Ar.IsLoading())
    {
        Attributes.GeomType = static_cast<EGeomType>(Type);
    }

    return Ar << Attributes.Offset << Attributes.Rotation << Attributes.Extent;
}

void UPhysicsAsset::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);
    const UPhysicsAsset* SrcAsset = static_cast<const UPhysicsAsset*>(Source);

    ConstraintSetups.Empty();

    for (FConstraintSetup* ConstraintSetup : SrcAsset->ConstraintSetups)
    {
        ConstraintSetups.Add(ConstraintSetup);
    }
}
