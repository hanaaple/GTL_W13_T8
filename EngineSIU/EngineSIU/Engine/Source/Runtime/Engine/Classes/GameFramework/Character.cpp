#include "Character.h"

#include "Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"


ACharacter::ACharacter()
{
    CapsuleComponent = AddComponent<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(CapsuleComponent);
    CapsuleComponent->bSimulate = true;
    CapsuleComponent->bApplyGravity = true;
    CapsuleComponent->bOverrideTransform = true;
    AggregateGeomAttributes geomAttributes;
    geomAttributes.GeomType = EGeomType::ECapsule;
    geomAttributes.Offset = FVector(0, 0, 88.0f);
    geomAttributes.Rotation = FRotator(90, 0, 0);
    geomAttributes.Extent = FVector(20.0f, 0.0f, 88.0f);
    CapsuleComponent->GetBodySetup()->GeomAttributes.Add(geomAttributes);
    MeshComponent = AddComponent<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
}
