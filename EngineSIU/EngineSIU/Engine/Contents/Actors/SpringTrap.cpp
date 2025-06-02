#include "SpringTrap.h"

#include "Components/PrimitiveComponent.h"
#include "World/World.h"

ASpringTrap::ASpringTrap() : Super()
{
    for (UPrimitiveComponent* PrimitiveComponent : GetComponentsByClass<UPrimitiveComponent>())
    {
        PrimitiveComponent->RigidBodyType = ERigidBodyType::KINEMATIC;
        PrimitiveComponent->bApplyGravity = false;
        PrimitiveComponent->SetSimulate(true);
    }
}

void ASpringTrap::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

    if (GetWorld()->WorldType == EWorldType::PIE && bEnabled)
    {
        UpdateState(DeltaTime);
    }
}

void ASpringTrap::TriggerTrap()
{
    if (GetWorld()->WorldType != EWorldType::PIE || bEnabled == false || TrapState != ETrapState::WaitShootCooldown)
    {
        return;
    }

    SetTrapState(ETrapState::Shoot);
}

void ASpringTrap::SetTrapState(ETrapState InTrapState)
{
    TrapState = InTrapState;

    if (TrapState == ETrapState::WaitShootCooldown || TrapState == ETrapState::WaitChargeCooldown)
    {
        CooldownTimer = 0;
    }
}
