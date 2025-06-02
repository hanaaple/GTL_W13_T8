#include "Sphere.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/FObjLoader.h"
#include "World/World.h"

ASphere::ASphere()
{
    StaticMeshComponent->SetStaticMesh(FObjManager::GetStaticMesh(L"Contents/Primitives/SpherePrimitive.obj"));
}

void ASphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
