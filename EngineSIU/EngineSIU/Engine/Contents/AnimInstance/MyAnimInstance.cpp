#include "MyAnimInstance.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimationRuntime.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Misc/FrameTime.h"
#include "Animation/AnimStateMachine.h"
#include "UObject/Casts.h"
#include "UObject/ObjectFactory.h"

UMyAnimInstance::UMyAnimInstance()
    : PrevAnim(nullptr)
    , CurrAnim(nullptr)
    , ElapsedTime(0.f)
    , PlayRate(1.f)
    , bLooping(true)
    , bPlaying(true)
    , bReverse(false)
    , LoopStartFrame(0)
    , LoopEndFrame(0)
    , CurrentKey(0)
    , BlendAlpha(0.f)
    , BlendStartTime(0.f)
    , BlendDuration(0.2f)
    , bIsBlending(false)
{
    StateMachine = FObjectFactory::ConstructObject<UAnimStateMachine>(this);

    // TODO: 하드코딩 덜어내기
    // 하지만 넌 지워지지 않겠지
    AnimSequenceMap["Idle"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/Armature|Idle"));
    //AnimSequenceMap["Jump"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/Armature|PaladinJump"));
    AnimSequenceMap["JumpUp"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/Armature|JumpUp"));
    AnimSequenceMap["Falling"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/Armature|Falling"));
    AnimSequenceMap["JumpDown"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/Armature|JumpDown"));
    AnimSequenceMap["Walking"] = Cast<UAnimSequence>(UAssetManager::Get().GetAnimation("Contents/Character/mixamo.com"));
}

void UMyAnimInstance::NativeInitializeAnimation()
{
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose)
{
    UAnimInstance::NativeUpdateAnimation(DeltaSeconds, OutPose);
    // StateMachine->ProcessState();
    
#pragma region MyAnim
    USkeletalMeshComponent* SkeletalMeshComp = GetSkelMeshComponent();
    
    if (!PrevAnim || !CurrAnim || !SkeletalMeshComp->GetSkeletalMeshAsset() || !SkeletalMeshComp->GetSkeletalMeshAsset()->GetSkeleton() || !bPlaying)
    {
        return;
    }
    
    ElapsedTime += DeltaSeconds;

    if (bIsBlending)
    {
        float BlendElapsed = ElapsedTime - BlendStartTime;
        BlendAlpha = FMath::Clamp(BlendElapsed / BlendDuration, 0.f, 1.f);

        if (BlendAlpha >= 1.f)
        {
            bIsBlending = false;
        }
    }
    else
    {
        BlendAlpha = 1.f;
    }
    
    // TODO: FPoseContext의 BoneContainer로 바꾸기
    const FReferenceSkeleton& RefSkeleton = this->GetCurrentSkeleton()->GetReferenceSkeleton();
    
    //if (PrevAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum()!= RefSkeleton.RawRefBoneInfo.Num() || CurrAnim->GetSkeleton()->GetReferenceSkeleton().GetRawBoneNum() != RefSkeleton.RawRefBoneInfo.Num())
    //{
    //    return;
    //}
    
    FPoseContext PrevPose(this);
    FPoseContext CurrPose(this);
    
    PrevPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    CurrPose.Pose.InitBones(RefSkeleton.RawRefBoneInfo.Num());
    for (int32 BoneIdx = 0; BoneIdx < RefSkeleton.RawRefBoneInfo.Num(); ++BoneIdx)
    {
        PrevPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
        CurrPose.Pose[BoneIdx] = RefSkeleton.RawRefBonePose[BoneIdx];
    }
    
    FAnimExtractContext ExtractA(GetElapsedTime(), false);
    FAnimExtractContext ExtractB(GetElapsedTime(), false);

    PrevAnim->GetAnimationPose(PrevPose, ExtractA);
    CurrAnim->GetAnimationPose(CurrPose, ExtractB);

    FAnimationRuntime::BlendTwoPosesTogether(CurrPose.Pose, PrevPose.Pose, BlendAlpha, OutPose.Pose);
#pragma endregion
}

void UMyAnimInstance::SetAnimState(FString InAnimState)
{
    if (CurrAnim != GetAnimSequence(InAnimState))
    {
        PrevAnim = CurrAnim;
        CurrAnim = GetAnimSequence(InAnimState);
        CurrentState = InAnimState;

        BlendAlpha = 0.f;
        BlendStartTime = ElapsedTime;

        bIsBlending = true;
    }
}

UAnimSequence* UMyAnimInstance::GetAnimSequence(FString InAnimState)
{
    return AnimSequenceMap.Contains(InAnimState) ? AnimSequenceMap[InAnimState] : nullptr;
}
