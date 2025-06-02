#pragma once
#include "GameFramework/Actor.h"

class ASpringTrap : public AActor
{
    DECLARE_CLASS(ASpringTrap, AActor)

protected:
    enum class ETrapState : uint8
    {
        WaitShootCooldown,
        Ready,
        Shoot,
        WaitChargeCooldown,
        Charge,
    };

public:
    ASpringTrap();
    virtual ~ASpringTrap() override = default;

    virtual void Tick(float DeltaTime) override;
    virtual void UpdateState(float DeltaTime) {}
    void TriggerTrap();

protected:
    void SetTrapState(ETrapState InTrapState);

public:
    UPROPERTY(EditAnywhere, bool, bEnabled, = false)
    UPROPERTY(EditAnywhere, float, ShootCoolDown, = 1)
    UPROPERTY(EditAnywhere, float, ChargeCoolDown, = 1)

    UPROPERTY(EditAnywhere, float, ShootSpeed, = 1)
    UPROPERTY(EditAnywhere, float, ChargeSpeed, = 1)

protected:
    ETrapState TrapState = ETrapState::Ready;

    float CooldownTimer = 0;
};
