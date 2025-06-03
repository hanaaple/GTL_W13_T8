#include "ShootingTrap.h"

#include "PhysicsManager.h"
#include "World/World.h"


void AShootingTrap::BeginPlay()
{
    Super::BeginPlay();
    
    Shoot();
}

void AShootingTrap::GetProperties(TMap<FString, FString>& OutProperties) const
{
    AActor::GetProperties(OutProperties);
    
    FString DuplicateTargetActorStr = FName().ToString();
    
    if (DuplicateTargetActor.Get())
    {
        DuplicateTargetActorStr = DuplicateTargetActor.Get()->GetName();
    }
    
    OutProperties.Add(TEXT("DuplicateTargetActor"), DuplicateTargetActorStr);

    OutProperties.Add(TEXT("LifeTime"), FString::SanitizeFloat(ActorLifeTime));
    OutProperties.Add(TEXT("ShootPowerScalar"), FString::SanitizeFloat(ShootForceScalar));
    OutProperties.Add(TEXT("SpawnInterval"), FString::SanitizeFloat(SpawnInterval));
    OutProperties.Add(TEXT("SpawnDirection"), SpawnDirection.ToString());
    OutProperties.Add(TEXT("SpawnLocation"), SpawnLocation.ToString());
    OutProperties.Add(TEXT("SpawnScale"), SpawnScale.ToString());
    OutProperties.Add(TEXT("ForceDistributionMin"), FString::SanitizeFloat(ForceDistribution.MinValue));
    OutProperties.Add(TEXT("ForceDistributionMax"), FString::SanitizeFloat(ForceDistribution.MaxValue));

    OutProperties.Add(TEXT("ForceDirectionDistributionMin"), ForceDirectionDistribution.MinValue.ToString());
    OutProperties.Add(TEXT("ForceDirectionDistributionMax"), ForceDirectionDistribution.MaxValue.ToString());
}

void AShootingTrap::SetProperties(const TMap<FString, FString>& InProperties)
{
    AActor::SetProperties(InProperties);

    const FString* TempStr = nullptr;
    
    if (InProperties.Contains("DuplicateTargetActor"))
    {
        UClass* InActorClass = UClass::FindClass(InProperties["DuplicateTargetActor"]);
        DuplicateTargetActor = InActorClass;
    }

    TempStr = InProperties.Find(TEXT("LifeTime"));
    if (TempStr)
    {
        ActorLifeTime = FString::ToFloat(*TempStr);
    }
    
    TempStr = InProperties.Find(TEXT("ShootPowerScalar"));
    if (TempStr)
    {
        ShootForceScalar = FString::ToFloat(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpawnInterval"));
    if (TempStr)
    {
        SpawnInterval = FString::ToFloat(*TempStr);
    }
    
    TempStr = InProperties.Find(TEXT("SpawnDirection"));
    if (TempStr)
    {
        SpawnDirection.InitFromString(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpawnLocation"));
    if (TempStr)
    {
        SpawnLocation.InitFromString(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpawnScale"));
    if (TempStr)
    {
        SpawnScale.InitFromString(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("ForceDistributionMin"));
    if (TempStr)
    {
        ForceDistribution.MinValue = FString::ToFloat(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("ForceDistributionMax"));
    if (TempStr)
    {
        ForceDistribution.MaxValue = FString::ToFloat(*TempStr);
    }
    
    TempStr = InProperties.Find(TEXT("ForceDirectionDistributionMin"));
    if (TempStr)
    {
        ForceDirectionDistribution.MinValue.InitFromString(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("ForceDirectionDistributionMax"));
    if (TempStr)
    {
        ForceDirectionDistribution.MaxValue.InitFromString(*TempStr);
    }
}

void AShootingTrap::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    for (AActor* PendingActor : PendingActors)
    {
        for (UPrimitiveComponent* PrimitiveComponent : PendingActor->GetComponentsByClass<UPrimitiveComponent>())
        {
            if (PrimitiveComponent->BodyInstance && PrimitiveComponent->BodyInstance->BIGameObject)
            {
                if (PrimitiveComponent->BodyInstance->BIGameObject->DynamicRigidBody)
                {
                    if (PrimitiveComponent->BodyInstance->BIGameObject->DynamicRigidBody->getType() == PxActorType::eRIGID_DYNAMIC)
                    {
                        FVector FVecForce = GetActorForwardVector() * (ShootForceScalar + ForceDistribution.GetValue());
                        PxVec3 Force = PxVec3(FVecForce.X, FVecForce.Y, FVecForce.Z);
                        PrimitiveComponent->BodyInstance->BIGameObject->DynamicRigidBody->addForce(Force, PxForceMode::eVELOCITY_CHANGE);
                    }
                }
            }
        }
    }
    PendingActors.Empty();

    t += DeltaTime;
    if (t > SpawnInterval)
    {
        t = 0;
        Shoot();
    }

    auto CopiedLifeTimes = SpawnedActors;

    for (auto& [Actor, SpawnInfo] : CopiedLifeTimes)
    {
        SpawnedActors[SpawnInfo.Actor].LifeTime += DeltaTime;
        if (SpawnedActors[SpawnInfo.Actor].LifeTime >= ActorLifeTime)
        {
            SpawnedActors.Remove(SpawnInfo.Actor);
            SpawnInfo.Actor->Destroy();
        }
    }
}

void AShootingTrap::Shoot()
{
    AActor* Actor = SpawnInternal();

    PendingActors.Add(Actor);
}

AActor* AShootingTrap::SpawnInternal()
{
    AActor* Actor = GetWorld()->SpawnActor(DuplicateTargetActor);
    Actor->SetActorLocation(GetActorLocation() + SpawnLocation);
    Actor->SetActorRotation(GetActorRotation() + SpawnDirection);
    Actor->SetActorScale(SpawnScale);
    SpawnedActors.Add(Actor, FSpawnInfo{ Actor, 0 });

    return Actor;
}
