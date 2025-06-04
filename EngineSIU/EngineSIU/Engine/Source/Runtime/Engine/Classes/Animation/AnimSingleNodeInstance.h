#pragma once
#include "AnimInstance.h"
#include "UObject/ObjectMacros.h"


class UAnimationAsset;
struct FTransform;

class UAnimSingleNodeInstance : public UAnimInstance
{
    DECLARE_CLASS(UAnimSingleNodeInstance, UAnimInstance)

public:
    UAnimSingleNodeInstance();

    virtual void SetAnimationAsset(UAnimationAsset* NewAsset, bool bIsLooping=true, float InPlayRate=1.f);

    virtual UFUNCTION(void, SetAnimState, FString InAnimState)
    
    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds, FPoseContext& OutPose) override;
    
    UAnimationAsset* GetAnimationAsset() const
    {
        return CurrentAsset;
    }

    UFUNCTION(void, SetPlaying, bool bIsPlaying)

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

    UFUNCTION(void, SetLooping, bool bIsLooping);

    UFUNCTION_CONST(bool, IsLooping);

    void SetElapsedTime(float InElapsedTime)
    {
        ElapsedTime = InElapsedTime;
    }

    float GetElapsedTime() const
    {
        return ElapsedTime;
    }

    UFUNCTION_CONST(float, GetPlayRate)
    
    UFUNCTION_CONST(float, GetAnimCurrentTime)

    UFUNCTION_CONST(float, GetAnimLength)

    UFUNCTION(void, SetPlayRate, float InPlayRate);

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
private:
    UPROPERTY(EditAnywhere | ShallowCopy, UAnimationAsset*, CurrentAsset, = nullptr)
 
    UPROPERTY(EditAnywhere, float, ElapsedTime, = 0.0f)

    UPROPERTY(EditAnywhere, float, PreviousTime, = 0.0f)

    UPROPERTY(EditAnywhere, float, PlayRate, = 1.0f)

    UPROPERTY(EditAnywhere, bool, bLooping, = true)

    UPROPERTY(EditAnywhere, bool, bPlaying, = true)

    UPROPERTY(EditAnywhere, bool, bReverse, = false)
    
    UPROPERTY(EditAnywhere, int32, LoopStartFrame, = 0)
    
    UPROPERTY(EditAnywhere, int32, LoopEndFrame, = 0)
    
    UPROPERTY(EditAnywhere, int32, CurrentKey, = 0)
};
