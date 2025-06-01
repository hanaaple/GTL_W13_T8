#include "ParticleEmitter.h"

#include "ParticleHelper.h"
#include "ParticleLODLevel.h"
#include "ParticleModule.h"
#include "ParticleModuleRequired.h"
#include "Container/ArrayHelper.h"
#include "Spawn/ParticleModuleSpawn.h"

UParticleEmitter::UParticleEmitter()
{
    UParticleLODLevel* NewParticleLODLevel = FObjectFactory::ConstructObject<UParticleLODLevel>(this); // LOD 하나만 쓰기 때문에 일단 이렇게 생성
    LODLevels.Add(NewParticleLODLevel);
}

void UParticleEmitter::CacheEmitterModuleInfo() // Not used
{
    // TODO: 언리얼 코드 참고
    
	ParticleSize = sizeof(FBaseParticle);

    ModuleOffsetMap.Empty();

    UParticleLODLevel* CurrentLODLevel = GetLODLevel(0);
    if (CurrentLODLevel == nullptr)
    {
        return;
    }

    ModuleOffsetMap.Add(CurrentLODLevel->RequiredModule, CurrentLODLevel->RequiredModule->ModulePayloadOffset);
    ModuleOffsetMap.Add(CurrentLODLevel->SpawnModule, CurrentLODLevel->SpawnModule->ModulePayloadOffset);

    for (const auto& Module : CurrentLODLevel->GetModules())
    {
        ModuleOffsetMap.Add(Module, Module->ModulePayloadOffset);
    }
}

void UParticleEmitter::DisplayProperty()
{
    Super::DisplayProperty();
    for (const auto& Property : StaticClass()->GetProperties())
    {
        Property->DisplayInImGui(this);
    }
}

UParticleLODLevel* UParticleEmitter::GetLODLevel(int32 LODIndex) const
{
    if (LODLevels.IsValidIndex(LODIndex))
    {
        return LODLevels[LODIndex];
    }
    return nullptr;
}

void UParticleEmitter::SerializeAsset(FArchive& Ar)
{
    uint8 Type = static_cast<uint8>(EmitterType);

    Ar << PeakActiveParticles << ParticleSize << Type;

    if (Ar.IsLoading())
    {
        EmitterType = static_cast<EDynamicEmitterType>(Type);
    }

    FArrayHelper::SerializePtrAsset(Ar, LODLevels, this);
}

void UParticleEmitter::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    UObject::DuplicateSubObjects(Source, InOuter, Duplicator);
    
}
