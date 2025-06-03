#include "Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"


ACharacter::ACharacter()
{
    CapsuleComponent = AddComponent<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(CapsuleComponent);

    MeshComponent = AddComponent<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
}
