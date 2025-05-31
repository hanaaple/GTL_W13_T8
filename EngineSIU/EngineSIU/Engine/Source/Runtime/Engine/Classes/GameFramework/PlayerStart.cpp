#include "PlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "InteractiveToolsFramework/BaseGizmos/GizmoArrowComponent.h"


APlayerStart::APlayerStart()
{
    CapsuleComponent = AddComponent<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetHalfHeight(9.2f);
    CapsuleComponent->SetRadius(4.0f);
    // CapsuleComponent->SetCollisionEnabled(false);
    CapsuleComponent->bDrawOnlyIfSelected = false;
    SetRootComponent(CapsuleComponent);
}
