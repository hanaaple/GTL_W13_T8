#include "RotatePatrolPlatform.h"

#include "Components/PrimitiveComponent.h"
#include "World/World.h"

void ARotatePatrolPlatform::BeginPlay()
{
    for (UPrimitiveComponent* PrimitiveComponent : GetComponentsByClass<UPrimitiveComponent>())
    {
        PrimitiveComponent->RigidBodyType = ERigidBodyType::KINEMATIC;
        PrimitiveComponent->bApplyGravity = false;
        PrimitiveComponent->SetSimulate(true);
    }
    
    Super::BeginPlay();

    Origin = GetActorRotation();

    SetActorRotation(Origin + TargetA);
}

void ARotatePatrolPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (GetWorld()->WorldType == EWorldType::PIE)
    {
        if (bJustRotate)
        {
            FQuat CurrentQuat = (TargetA + Origin).Quaternion();
        
            FQuat TargetQuat = (TargetB + Origin).Quaternion();

            FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

            FVector RotationAxis;
            float AngleDifferenceRadians;
            DeltaQuat.ToAxisAndAngle(RotationAxis, AngleDifferenceRadians);
            
            float DeltaAngleDegreesThisTick = AngularSpeed * DeltaTime;
            float DeltaAngleRadiansThisTick = FMath::DegreesToRadians(DeltaAngleDegreesThisTick);

            FQuat StepQuat(RotationAxis, DeltaAngleRadiansThisTick);
    
            AddActorRotation(StepQuat);
        }
        else
        {
            FRotator ActorRotation = GetActorRotation();
            FQuat QuatA = (TargetA + Origin).Quaternion();
            FQuat QuatB = (TargetB + Origin).Quaternion();
            FQuat ActorQuat = ActorRotation.Quaternion();

            float DeltaARadian = (QuatA * ActorQuat.Inverse()).GetAngle();
            float DeltaADegrees = FMath::RadiansToDegrees(DeltaARadian);

            float DeltaBRadian = (QuatB * ActorQuat.Inverse()).GetAngle();
            float DeltaBDegrees = FMath::RadiansToDegrees(DeltaBRadian);
        
            if (DeltaADegrees < 1)
            {
                Target = ETarget::TargetB;
            }
            else if (DeltaBDegrees < 1)
            {
                Target = ETarget::TargetA;
            }

            FQuat CurrentQuat = ActorRotation.Quaternion();
        
            FRotator TargetRot = Target == ETarget::TargetA ? TargetA : TargetB;
            TargetRot += Origin;
        
            FQuat TargetQuat = TargetRot.Quaternion();

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
}

void ARotatePatrolPlatform::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);

    OutProperties.Add("bJustRotate", bJustRotate ? TEXT("true") : TEXT("false"));
    OutProperties.Add("PlatformAngularSpeed", FString::SanitizeFloat(AngularSpeed));
    OutProperties.Add("PlatformTargetA", TargetA.ToString());
    OutProperties.Add("PlatformTargetB", TargetB.ToString());
}

void ARotatePatrolPlatform::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);

    const FString* TempStr = nullptr;

    TempStr = InProperties.Find(TEXT("bJustRotate"));
    if (TempStr)
    {
        bJustRotate = (*TempStr == TEXT("true"));
    }
    
    TempStr = InProperties.Find(TEXT("PlatformAngularSpeed"));
    if (TempStr)
    {
        AngularSpeed = FCString::Atof(**TempStr);
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
