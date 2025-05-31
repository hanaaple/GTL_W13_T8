#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"


AStaticMeshActor::AStaticMeshActor()
{
    // 액터가 생성자에서 컴포넌트를 만들면 이름을 넣어줘야 정상적으로 세이브 된다.
    StaticMeshComponent = AddComponent<UStaticMeshComponent>("StaticMeshComponent_0");
    RootComponent = StaticMeshComponent;
}

UStaticMeshComponent* AStaticMeshActor::GetStaticMeshComponent() const
{
    return StaticMeshComponent;
}


