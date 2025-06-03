#pragma once
#include "Distribution/DistributionFloat.h"
#include "Distribution/DistributionVector.h"
#include "GameFramework/Actor.h"
#include "Template/SubclassOf.h"


class AShootingTrap : public AActor
{
    DECLARE_CLASS(AShootingTrap, AActor)

public:
    AShootingTrap() = default;
    virtual ~AShootingTrap() override = default;

    virtual void BeginPlay() override;

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;


    virtual void Tick(float DeltaTime) override;
    void Shoot();


protected:
    AActor* SpawnInternal();

public:


    UPROPERTY(EditAnywhere, float, ActorLifeTime, = 1)
    UPROPERTY(EditAnywhere, float, SpawnInterval, = 1)
    UPROPERTY(EditAnywhere, float, ShootForceScalar, = 1)
    UPROPERTY(EditAnywhere, FRotator, SpawnDirection, = FRotator::ZeroRotator)
    UPROPERTY(EditAnywhere, FVector, SpawnLocation, = FVector::ZeroVector)
    UPROPERTY(EditAnywhere, FVector, SpawnScale, = FVector::OneVector)

    UPROPERTY(EditAnywhere, FDistributionFloat, ForceDistribution, = FDistributionFloat(0, 0))
    UPROPERTY(EditAnywhere, FDistributionVector, ForceDirectionDistribution, = FDistributionVector(FVector::ZeroVector, FVector::ZeroVector))
    

    UPROPERTY(EditAnywhere, TSubclassOf<AActor>, DuplicateTargetActor, = nullptr)

    
private:

    struct FSpawnInfo
    {
        AActor* Actor;
        float LifeTime;
    };
    float t = 0;
    TMap<AActor*, FSpawnInfo> SpawnedActors;

    TArray<AActor*> PendingActors;
};
