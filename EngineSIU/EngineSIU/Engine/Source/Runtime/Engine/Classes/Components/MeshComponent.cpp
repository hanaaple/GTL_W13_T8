#include "MeshComponent.h"

#include "CoreMiscDefines.h"
#include "UObject/Casts.h"
#include "UObject/FObjectDuplicator.h"


void UMeshComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
}

void UMeshComponent::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
}

UMaterial* UMeshComponent::GetMaterial(uint32 ElementIndex) const
{
    if (OverrideMaterials.IsValidIndex(ElementIndex))
        return OverrideMaterials[ElementIndex];

    return nullptr;
}

uint32 UMeshComponent::GetMaterialIndex(FName MaterialSlotName) const
{
    // This function should be overridden
    return INDEX_NONE;
}

UMaterial* UMeshComponent::GetMaterialByName(FName MaterialSlotName) const
{
    int32 MaterialIndex = GetMaterialIndex(MaterialSlotName);
    if (MaterialIndex < 0)
        return nullptr;
    return GetMaterial(MaterialIndex);
}

TArray<FName> UMeshComponent::GetMaterialSlotNames() const
{
    return TArray<FName>();
}

void UMeshComponent::SetMaterial(uint32 ElementIndex, UMaterial* Material)
{
    if (OverrideMaterials.IsValidIndex(ElementIndex) == false) return;

    OverrideMaterials[ElementIndex] = Material;
}

void UMeshComponent::SetMaterialByName(FName MaterialSlotName, UMaterial* Material)
{
    int32 MaterialIndex = GetMaterialIndex(MaterialSlotName);
    if (MaterialIndex < 0)
        return;

    SetMaterial(MaterialIndex, Material);
}

void UMeshComponent::GetUsedMaterials(TArray<UMaterial*>& Out) const
{
    for (int32 ElementIndex = 0; ElementIndex < GetNumMaterials(); ElementIndex++)
    {
        if (UMaterial* Material = GetMaterial(ElementIndex))
        {
            Out.Add(Material);
        }
    }
}

void UMeshComponent::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);
    const UMeshComponent* SrcComp = static_cast<const UMeshComponent*>(Source);
    OverrideMaterials.Empty();

    for (UMaterial* Material : SrcComp->OverrideMaterials)
    {
        if (Material != nullptr)
        {
            UMaterial* DuplicatedMaterial = static_cast<UMaterial*>(Duplicator.DuplicateObject(Material));
            OverrideMaterials.Add(DuplicatedMaterial);
        }
    }
}
