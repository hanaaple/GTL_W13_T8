#pragma once
#include "Animation/Animinstance.h"
#include "Animation/AnimStateMachine.h"
#include "UObject/ObjectMacros.h"

class UAnimSequence;
class UAnimationAsset;

class UMyAnimInstance : public UAnimInstance
{
    DECLARE_CLASS(UMyAnimInstance, UAnimInstance)

public:
    UMyAnimInstance();
    
    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose) override;
    
    void SetPlaying(bool bIsPlaying)
    {
        bPlaying = bIsPlaying;
    }

    bool IsPlaying() const
    {
        return bPlaying;
    }

    void SetReverse(bool bIsReverse)
    {
        bReverse = bIsReverse;
    }
    
    bool IsReverse() const
    {
        return bReverse;
    }

    void SetLooping(bool bIsLooping)
    {
        bLooping = bIsLooping;
    }

    bool IsLooping() const
    {
        return bLooping;
    }

    void SetElapsedTime(float InElapsedTime)
    {
        ElapsedTime = InElapsedTime;
    }

    float GetElapsedTime() const
    {
        return ElapsedTime;
    }

    float GetPlayRate() const
    {
        return PlayRate;
    }

    void SetPlayRate(float InPlayRate)
    {
        PlayRate = InPlayRate;
    }

    int32 GetLoopStartFrame() const
    {
        return LoopStartFrame;
    }

    void SetLoopStartFrame(int32 InLoopStartFrame)
    {
        LoopStartFrame = InLoopStartFrame;
    }

    int32 GetLoopEndFrame() const
    {
        return LoopEndFrame;
    }

    void SetLoopEndFrame(int32 InLoopEndFrame)
    {
        LoopEndFrame = InLoopEndFrame;
    }

    int GetCurrentKey() const
    {
        return CurrentKey;
    }

    void SetCurrentKey(int InCurrentKey)
    {
        CurrentKey = InCurrentKey;
    }

    virtual void SetAnimState(EAnimState InAnimState) override;

    UAnimSequence* GetAnimForState(EAnimState InAnimState);

    virtual UAnimStateMachine* GetStateMachine() const override { return StateMachine; }

    virtual UAnimSequence* GetCurrAnim() const override { return CurrAnim; }

    virtual UAnimSequence* GetPrevAnim() const override { return PrevAnim; }

    virtual float GetBlendDuration() const override { return BlendDuration; }

    virtual void SetBlendDuration(float InBlendDuration) override { BlendDuration = InBlendDuration; }
private:
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, IDLE, = nullptr )
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, Dance, = nullptr )
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, SlowRun, = nullptr )
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, NarutoRun, = nullptr )
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, FastRun, = nullptr )

    UPROPERTY(EditAnywhere, float, ElapsedTime, = 0.0f)

    UPROPERTY(EditAnywhere, float, PlayRate, = 1.0f)

    UPROPERTY(EditAnywhere, bool, bLooping, = true)

    UPROPERTY(EditAnywhere, bool, bPlaying, = true)

    UPROPERTY(EditAnywhere, bool, bReverse, = false)
    
    UPROPERTY(EditAnywhere, int32, LoopStartFrame, = 0)
    
    UPROPERTY(EditAnywhere, int32, LoopEndFrame, = 0)
    
    UPROPERTY(EditAnywhere, int32, CurrentKey, = 0)

    UPROPERTY(EditAnywhere, float, BlendAlpha, = 0.f)

    UPROPERTY(EditAnywhere, float, BlendStartTime, = 0.f)

    UPROPERTY(EditAnywhere, float, BlendDuration, = 0.2f)

    UPROPERTY(EditAnywhere, bool, bIsBlending, = false)

    UPROPERTY(EditAnywhere | ShallowCopy, UAnimStateMachine*, StateMachine, = nullptr)
    
    UAnimSequence* PrevAnim;
    
    UAnimSequence* CurrAnim;

};
