#include "RotatePatrolPlatform.h"

#include "Components/PrimitiveComponent.h"
#include "World/World.h"

ARotatePatrolPlatform::ARotatePatrolPlatform()
{
    for (UPrimitiveComponent* PrimitiveComponent : GetComponentsByClass<UPrimitiveComponent>())
    {
        PrimitiveComponent->RigidBodyType = ERigidBodyType::KINEMATIC;
        PrimitiveComponent->bApplyGravity = false;
        PrimitiveComponent->SetSimulate(true);
    }

    TargetRot = &TargetA;
    SetActorRotation(TargetA);
}

void ARotatePatrolPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (GetWorld()->WorldType == EWorldType::PIE)
    {
        FRotator ActorRotation = GetActorRotation();
        FQuat QuatA = TargetA.Quaternion();
        FQuat QuatB = TargetB.Quaternion();
        FQuat ActorQuat = ActorRotation.Quaternion();

        float DeltaARadian = (QuatA * ActorQuat.Inverse()).GetAngle();
        float DeltaADegrees = FMath::RadiansToDegrees(DeltaARadian);

        float DeltaBRadian = (QuatB * ActorQuat.Inverse()).GetAngle();
        float DeltaBDegrees = FMath::RadiansToDegrees(DeltaBRadian);
        
        if (DeltaADegrees < 1)
        {
            TargetRot = &TargetB;
        }
        else if (DeltaBDegrees < 1)
        {
            TargetRot = &TargetA;
        }

        FQuat CurrentQuat = ActorRotation.Quaternion();
        FQuat TargetQuat = TargetRot->Quaternion();

        FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

        FVector RotationAxis;
        float AngleDifferenceRadians;
        DeltaQuat.ToAxisAndAngle(RotationAxis, AngleDifferenceRadians);

        float DeltaAngleDegreesThisTick = AngularSpeed * DeltaTime;
        float DeltaAngleRadiansThisTick = FMath::DegreesToRadians(DeltaAngleDegreesThisTick);

        float StepAngleRadians = FMath::Min(DeltaAngleRadiansThisTick , AngleDifferenceRadians);

        FQuat StepQuat(RotationAxis, StepAngleRadians);
    
        AddActorRotation(StepQuat);
    }
}
