#include "Fish.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


AFish::AFish()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>("StaticMeshComponent_0");
    SetRootComponent(StaticMeshComponent);
    SkeletalMeshComponent = AddComponent<USkeletalMeshComponent>("SkeletalMeshComponent_0");
    SkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
    SetActorTickInEditor(false);
}

void AFish::PostSpawnInitialize()
{
    Super::PostSpawnInitialize();
    SetActorLabel(TEXT("OBJ_FISH"));
}

void AFish::BeginPlay()
{
    AggregateGeomAttributes GeomAttributes;
    GeomAttributes.GeomType = EGeomType::ECapsule;
    GeomAttributes.Offset = FVector(0, 8, 0);
    GeomAttributes.Rotation = FRotator(0, 90, 0); 
    GeomAttributes.Extent = FVector(4.8, 1, 16.8) * GetActorScale();

    StaticMeshComponent->RigidBodyType = ERigidBodyType::DYNAMIC;
    StaticMeshComponent->bApplyGravity = true;

    StaticMeshComponent->GetBodySetup()->GeomAttributes.Empty();
    StaticMeshComponent->GetBodySetup()->GeomAttributes.Add(GeomAttributes);
    
    
    SkeletalMeshComponent->SetRelativeScale3D(FVector(.1f, .1f, .1f));
    
    SkeletalMeshComponent->SetLooping(true);
    SkeletalMeshComponent->SetPlaying(true);
    SkeletalMeshComponent->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh("Contents/Fish/Fish"));

    UAnimationAsset* Animation = UAssetManager::Get().GetAnimation(FName("Contents/Fish/아마튜어|아마튜어.001"));
    
    UAnimSequence* AnimSeq = Cast<UAnimSequence>(Animation);
    if (Animation && AnimSeq)
    {
        const bool bWasLooping = SkeletalMeshComponent->IsLooping();
        const bool bWasPlaying = SkeletalMeshComponent->IsPlaying();

        SkeletalMeshComponent->SetAnimation(AnimSeq);
                            
        SkeletalMeshComponent->SetLooping(bWasLooping);
        if (bWasPlaying)
        {
            SkeletalMeshComponent->Play(bWasLooping);
        }
        else
        {
            SkeletalMeshComponent->Stop();
        }
        SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        SkeletalMeshComponent->PlayAnimation(Animation, true);
        SkeletalMeshComponent->DEBUG_SetAnimationEnabled(true);
        SkeletalMeshComponent->SetPlaying(true);
    }

    StaticMeshComponent->SetSimulate(false);
    StaticMeshComponent->SetSimulate(true);

    Super::BeginPlay();
}
