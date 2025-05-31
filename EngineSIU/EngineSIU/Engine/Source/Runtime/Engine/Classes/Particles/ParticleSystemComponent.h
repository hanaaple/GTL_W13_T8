#pragma once
#include "Components/PrimitiveComponent.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class FParticleDynamicData;
class UParticleSystem;
class UParticleEmitter;
struct FParticleEmitterInstance;
struct FDynamicEmitterDataBase;

class UFXSystemComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UFXSystemComponent, UPrimitiveComponent)

public:
    UFXSystemComponent() = default;
    virtual ~UFXSystemComponent() override = default;
};

class UParticleSystemComponent : public UFXSystemComponent
{
    DECLARE_CLASS(UParticleSystemComponent, UFXSystemComponent)

public:
    UParticleSystemComponent();
    virtual ~UParticleSystemComponent() override = default;

    //virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

public:
    void InitializeSystem();
    void CreateAndAddSpriteEmitterInstance(UParticleEmitter* EmitterTemplate);
    void CreateAndAddMeshEmitterInstance(UParticleEmitter* EmitterTemplate);

    void UpdateDynamicData();
    FParticleDynamicData* CreateDynamicData();

    UParticleSystem* GetParticleSystem() const { return Template; }
    void SetParticleSystem(UParticleSystem* InParticleSystem) { Template = InParticleSystem; }

    FParticleDynamicData* GetParticleDynamicData() const { return ParticleDynamicData; }
    
    void ReBuildInstancesMemoryLayout();
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

    UPROPERTY(EditAnywhere, float, AccumTickTime, = 0.0f)

private:
    TArray<FParticleEmitterInstance*> EmitterInstances;

    UPROPERTY_WITH_FLAGS(EditAnywhere | ShallowCopy, UParticleSystem*, Template)

    TArray<FDynamicEmitterDataBase*> EmitterRenderData;

    FParticleDynamicData* ParticleDynamicData = nullptr;
};
