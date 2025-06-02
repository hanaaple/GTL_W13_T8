#include "RotateSpringTrap.h"

ARotateSpringTrap::ARotateSpringTrap() : Super()
{
    Origin = GetActorRotation();
}

void ARotateSpringTrap::UpdateState(float DeltaTime)
{
    if (bEnabled == false)
    {
        return;
    }

    Super::UpdateState(DeltaTime);

    if (TrapState == ETrapState::WaitShootCooldown)
    {
        CooldownTimer += DeltaTime;
        if (CooldownTimer >= ShootCoolDown)
        {
            SetTrapState(ETrapState::Ready);
        }
    }
    else if (TrapState == ETrapState::Ready)
    {
        // Test Code
        SetTrapState(ETrapState::Shoot);
    }
    else if (TrapState == ETrapState::Shoot)
    {
        FRotator ActorRotation = GetActorRotation();
        
        FQuat CurrentQuat = ActorRotation.Quaternion();
        FQuat TargetQuat = Target.Quaternion();

        FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

        FVector RotationAxis;
        float AngleDifferenceRadians;
        DeltaQuat.ToAxisAndAngle(RotationAxis, AngleDifferenceRadians);

        float DeltaAngleDegreesThisTick = ShootSpeed * DeltaTime;
        float DeltaAngleRadiansThisTick = FMath::DegreesToRadians(DeltaAngleDegreesThisTick);

        float StepAngleRadians = FMath::Min(DeltaAngleRadiansThisTick , AngleDifferenceRadians);

        FQuat StepQuat(RotationAxis, StepAngleRadians);

        AddActorRotation(StepQuat);

        if (AngleDifferenceRadians < DeltaAngleRadiansThisTick)
        {
            SetTrapState(ETrapState::WaitChargeCooldown);
        }
    }
    else if (TrapState == ETrapState::WaitChargeCooldown)
    {
        CooldownTimer += DeltaTime;
        if (CooldownTimer >= ChargeCoolDown)
        {
            SetTrapState(ETrapState::Charge);
        }
    }
    else if (TrapState == ETrapState::Charge)
    {
        FRotator ActorRotation = GetActorRotation();
        
        FQuat CurrentQuat = ActorRotation.Quaternion();
        FQuat TargetQuat = Origin.Quaternion();

        FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

        FVector RotationAxis;
        float AngleDifferenceRadians;
        DeltaQuat.ToAxisAndAngle(RotationAxis, AngleDifferenceRadians);

        float DeltaAngleDegreesThisTick = ChargeSpeed * DeltaTime;
        float DeltaAngleRadiansThisTick = FMath::DegreesToRadians(DeltaAngleDegreesThisTick);

        float StepAngleRadians = FMath::Min(DeltaAngleRadiansThisTick , AngleDifferenceRadians);

        FQuat StepQuat(RotationAxis, StepAngleRadians);

        AddActorRotation(StepQuat);

        if (AngleDifferenceRadians < DeltaAngleRadiansThisTick)
        {
            SetTrapState(ETrapState::WaitShootCooldown);
        }
    }
}
