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

    TargetPos = &TargetA;
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
            TargetPos = &TargetB;
        }
        else if (FVector::Distance(TargetB, ActorLocation) < 1)
        {
            TargetPos = &TargetA;
        }

        FVector TranslateVector = (*TargetPos - ActorLocation).GetSafeNormal() * Speed * DeltaTime;

        if ((*TargetPos - ActorLocation).Length() < TranslateVector.Length())
        {
            TranslateVector = TranslateVector.GetSafeNormal() * (*TargetPos - ActorLocation).Length();
        }
                
        AddActorLocation(TranslateVector);
    }
}
