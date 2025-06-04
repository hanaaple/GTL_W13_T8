#include "AnimInstance.h"

#include "AnimSequence.h"
#include "AnimSoundNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/Casts.h"

void UAnimInstance::InitializeAnimation()
{
    USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
    if (OwnerComponent->GetSkeletalMeshAsset() != nullptr)
    {
        CurrentSkeleton = OwnerComponent->GetSkeletalMeshAsset()->GetSkeleton();
    }
    else
    {
        CurrentSkeleton = nullptr;
    }
}

void UAnimInstance::UpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    NativeUpdateAnimation(DeltaSeconds, OutPose);
}

void UAnimInstance::NativeInitializeAnimation()
{
}

void UAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
}

void UAnimInstance::SetAnimState(FString InAnimState)
{
}

UAnimSequence* UAnimInstance::GetAnimSequence(FString InAnimState)
{
    return nullptr;
}

UAnimStateMachine* UAnimInstance::GetStateMachine() const
{
    return nullptr;
}

UAnimSequence* UAnimInstance::GetCurrAnim() const
{
    return nullptr;
}

UAnimSequence* UAnimInstance::GetPrevAnim() const
{
    return nullptr;
}

float UAnimInstance::GetBlendDuration() const
{
    return 0.f;
}

void UAnimInstance::SetBlendDuration(float InBlendDuration)
{
}

FString UAnimInstance::GetCurrentState() const
{
    return CurrentState;
}

TMap<FString, UAnimSequence*> UAnimInstance::GetAnimSequenceMap() const
{
    return AnimSequenceMap;
}

void UAnimInstance::AddSoundNotify(UAnimSequence* TargetAnim, FName SoundKey, FName SoundNotifyName, float MagicNumber) const
{
    if (TargetAnim == nullptr)
    {
        return;
    }

    FName NewTrackName = FName("SoundTrack");
    
    int32 NewTrackIdx = INDEX_NONE;
    TargetAnim->AddNotifyTrack(NewTrackName, NewTrackIdx);
    if (NewTrackIdx != -1)
    {
        int32 NotifyIndex = TargetAnim->Notifies.Num();
        if (!TargetAnim->GetNotifyEvent(NotifyIndex))
        {
            TargetAnim->AddNotifyEvent(0, MagicNumber, 0, SoundNotifyName, NotifyIndex);
        }
        if (FAnimNotifyEvent* NotifyEvent = TargetAnim->GetNotifyEvent(NotifyIndex))
        {
            auto* Notify = FObjectFactory::ConstructObject<UAnimSoundNotify>(nullptr);
            Notify->SetSoundName(SoundKey);
            NotifyEvent->SetAnimNotify(Notify);
        }
    }
}

USkeletalMeshComponent* UAnimInstance::GetSkelMeshComponent() const
{
    return Cast<USkeletalMeshComponent>(GetOuter());
}

void UAnimInstance::TriggerAnimNotifies(float DeltaSeconds)
{
}

void UAnimInstance::RegisterAnimState(FString InStateName, FString InAnimName)
{
    if (InStateName == "None")
    {
        return;
    }
    UAnimSequence* AnimSequence = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation(InAnimName));
    if (AnimSequence == nullptr)
    {
        UE_LOG(ELogLevel::Warning, "Failed to register Anim State %s as %s", GetData(InAnimName), GetData(InStateName));
        return;
    }
    if (AnimSequenceMap.Contains(InStateName))
    {
        UE_LOG(ELogLevel::Warning, "State Name %s is exist. Not Happened.", GetData(InStateName));
        return;
    }
    AnimSequenceMap.Emplace(InStateName, AnimSequence);
}

void UAnimInstance::UnregisterAnimState(FString InStateName)
{
    AnimSequenceMap.Remove(InStateName);
}
