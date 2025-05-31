#include "CubeActor.h"
#include "Components/BoxComponent.h"

ACubeActor::ACubeActor()
{
    BoxComponent = AddComponent<UBoxComponent>();
}

UBoxComponent* ACubeActor::GetShapeComponent() const
{
    return BoxComponent;
}
