#include "SpringTrap.h"

#include "Components/PrimitiveComponent.h"
#include "World/World.h"

void ASpringTrap::PostSpawnInitialize()
{
    AActor::PostSpawnInitialize();
    
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

void ASpringTrap::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);

    OutProperties.Add("SpringTrapEnabled", bEnabled ? TEXT("true") : TEXT("false"));
    OutProperties.Add("SpringTrapInitialCooldown", FString::SanitizeFloat(InitialCooldown));
    OutProperties.Add("SpringTrapShootCoolDown", FString::SanitizeFloat(ShootCoolDown));
    OutProperties.Add("SpringTrapChargeCoolDown", FString::SanitizeFloat(ChargeCoolDown));
    OutProperties.Add("SpringTrapShootSpeed", FString::SanitizeFloat(ShootSpeed));
    OutProperties.Add("SpringTrapChargeSpeed", FString::SanitizeFloat(ChargeSpeed));
}

void ASpringTrap::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);

    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("SpringTrapEnabled"));
    if (TempStr)
    {
        bEnabled = (*TempStr == TEXT("true"));
    }

    TempStr = InProperties.Find(TEXT("SpringTrapInitialCooldown"));
    if (TempStr)
    {
        InitialCooldown = FCString::Atof(**TempStr);
    }
    
    TempStr = InProperties.Find(TEXT("SpringTrapShootCoolDown"));
    if (TempStr)
    {
        ShootCoolDown = FCString::Atof(**TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpringTrapChargeCoolDown"));
    if (TempStr)
    {
        ChargeCoolDown = FCString::Atof(**TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpringTrapShootSpeed"));
    if (TempStr)
    {
        ShootSpeed = FCString::Atof(**TempStr);
    }

    TempStr = InProperties.Find(TEXT("SpringTrapChargeSpeed"));
    if (TempStr)
    {
        ChargeSpeed = FCString::Atof(**TempStr);
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
