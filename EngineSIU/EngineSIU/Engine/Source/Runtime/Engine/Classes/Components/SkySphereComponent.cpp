#include "SkySphereComponent.h"
#include "UObject/Casts.h"


USkySphereComponent::USkySphereComponent()
{
    SetType(StaticClass()->GetName());
}

void USkySphereComponent::TickComponent(float DeltaTime)
{
    UOffset += 0.005f;
    VOffset += 0.005f;
    Super::TickComponent(DeltaTime);
}
