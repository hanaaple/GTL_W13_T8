#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "ParticleHelper.h"

class UParticleModule;
class UParticleLODLevel;

class UParticleEmitter : public UObject
{
    DECLARE_CLASS(UParticleEmitter, UObject)

public:
    UParticleEmitter();
    virtual ~UParticleEmitter() override = default;

    void CacheEmitterModuleInfo();

    virtual void DisplayProperty() override;

    TArray<UParticleLODLevel*> GetLODLevels() const { return LODLevels; }
    TMap<UParticleModule*, uint32> ModuleOffsetMap; // Not used
    UParticleLODLevel* GetLODLevel(int32 LODIndex) const;

    virtual void SerializeAsset(FArchive& Ar) override;
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

    UPROPERTY_WITH_FLAGS(EditAnywhere, FName, EmitterName, = "Default")

    UPROPERTY(int32, PeakActiveParticles, = 0)

    // Below is information updated by calling CacheEmitterModuleInfo
    UPROPERTY(EditAnywhere, int32, ParticleSize, = 0)

    UPROPERTY(EditAnywhere, EDynamicEmitterType, EmitterType, = EDynamicEmitterType::DET_Unknown)
private:
    TArray<UParticleLODLevel*> LODLevels; // 현재는 Level 0만 사용
};
