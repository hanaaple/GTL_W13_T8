#include "TranslateSpringTrap.h"

void ATranslateSpringTrap::BeginPlay()
{
    Super::BeginPlay();
    Origin = GetActorLocation();
}

void ATranslateSpringTrap::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);

    OutProperties.Add("SpringTrapTarget", Target.ToString());
}

void ATranslateSpringTrap::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
    
    const FString* TempStr = nullptr;
    TempStr = InProperties.Find(TEXT("SpringTrapTarget"));
    if (TempStr)
    {
        Target.InitFromString(*TempStr);
    }
}

void ATranslateSpringTrap::UpdateState(float DeltaTime)
{
    if (bEnabled == false)
    {
        return;
    }
    
    if (InitialCooldown > 0.f)
    {
        InitialCooldown -= DeltaTime;
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
        FVector TranslateVector = ((Target + Origin) - ActorLocation).GetSafeNormal() * ShootSpeed * DeltaTime;

        if (((Target + Origin) - ActorLocation).Length() < TranslateVector.Length())
        {
            TranslateVector = (Target + Origin) - ActorLocation;
        }
        
        AddActorLocation(TranslateVector);

        if (FVector::Distance((Target + Origin), ActorLocation) < 0.1)
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
