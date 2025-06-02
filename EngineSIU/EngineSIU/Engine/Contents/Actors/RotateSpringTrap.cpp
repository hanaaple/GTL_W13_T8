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
    else if (TrapState == ETrapState::Shoot)
    {
        // (Target - Origin).Quaternion()
        // AddActorRotation()
        // ShootSpeed
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
        // TODO
        // ChargeSpeed
    }
}
