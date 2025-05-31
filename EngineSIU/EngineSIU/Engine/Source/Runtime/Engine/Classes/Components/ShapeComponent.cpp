
#include "ShapeComponent.h"

UShapeComponent::UShapeComponent()
{
}

void UShapeComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    UpdateOverlaps();
}
