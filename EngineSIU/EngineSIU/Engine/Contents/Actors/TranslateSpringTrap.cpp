#include "TranslateSpringTrap.h"

ATranslateSpringTrap::ATranslateSpringTrap() : Super()
{
    Origin = GetActorLocation();
}

void ATranslateSpringTrap::UpdateState(float DeltaTime)
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
        FVector ActorLocation = GetActorLocation();
        FVector TranslateVector = (Target - ActorLocation).GetSafeNormal() * ShootSpeed * DeltaTime;

        if ((Target - ActorLocation).Length() < TranslateVector.Length())
        {
            TranslateVector = Target - ActorLocation;
        }
        
        AddActorLocation(TranslateVector);

        if (FVector::Distance(Target, ActorLocation) < 0.1)
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
        FVector ActorLocation = GetActorLocation();
        
        FVector TranslateVector = (Origin - ActorLocation).GetSafeNormal() * ChargeSpeed * DeltaTime;

        if ((Origin - ActorLocation).Length() < TranslateVector.Length())
        {
            TranslateVector = Origin - ActorLocation;
        }
        
        AddActorLocation(TranslateVector);

        if (TranslateVector.Length() < 0.1)
        {
            SetTrapState(ETrapState::WaitShootCooldown);
        }
    }
}
