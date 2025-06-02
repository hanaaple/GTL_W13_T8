#include "TranslatePatrolPlatform.h"

#include "Components/StaticMeshComponent.h"
#include "World/World.h"

ATranslatePatrolPlatform::ATranslatePatrolPlatform()
{
    for (UPrimitiveComponent* PrimitiveComponent : GetComponentsByClass<UPrimitiveComponent>())
    {
        PrimitiveComponent->RigidBodyType = ERigidBodyType::KINEMATIC;
        PrimitiveComponent->bApplyGravity = false;
        PrimitiveComponent->SetSimulate(true);
    }

    SetActorLocation(TargetA);
}

void ATranslatePatrolPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (GetWorld()->WorldType == EWorldType::PIE)
    {
        FVector ActorLocation = GetActorLocation();
        if (FVector::Distance(TargetA, ActorLocation) < 1)
        {
            Target = ETarget::TargetB;
        }
        else if (FVector::Distance(TargetB, ActorLocation) < 1)
        {
            Target = ETarget::TargetA;
        }

        FVector TargetPos = Target == ETarget::TargetA ? TargetA : TargetB;
        
        FVector TranslateVector = (TargetPos - ActorLocation).GetSafeNormal() * Speed * DeltaTime;

        if ((TargetPos - ActorLocation).Length() < TranslateVector.Length())
        {
            TranslateVector = TargetPos - ActorLocation;
        }
                
        AddActorLocation(TranslateVector);
    }
}

void ATranslatePatrolPlatform::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);

    OutProperties.Add("PlatformSpeed", FString::SanitizeFloat(Speed));
    OutProperties.Add("PlatformTargetA", TargetA.ToString());
    OutProperties.Add("PlatformTargetB", TargetB.ToString());
}

void ATranslatePatrolPlatform::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);

    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("PlatformSpeed"));
    if (TempStr)
    {
        Speed = FCString::Atof(**TempStr);
    }

    TempStr = InProperties.Find(TEXT("PlatformTargetA"));
    if (TempStr)
    {
        TargetA.InitFromString(*TempStr);
    }

    TempStr = InProperties.Find(TEXT("PlatformTargetB"));
    if (TempStr)
    {
        TargetB.InitFromString(*TempStr);
    }
}
