#include "Material.h"
#include "UObject/Casts.h"


// UObject* UMaterial::Duplicate(UObject* InOuter)
// {
//     ThisClass* NewMaterial = Cast<ThisClass>(Super::Duplicate(InOuter));
//
//     NewMaterial->MaterialInfo = MaterialInfo;
//
//     return NewMaterial;
// }

void UMaterial::SerializeAsset(FArchive& Ar)
{
    MaterialInfo.Serialize(Ar);
}

void UMaterial::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);
    const UMaterial* SrcMat = static_cast<const UMaterial*>(Source);

    MaterialInfo = SrcMat->MaterialInfo;
}
