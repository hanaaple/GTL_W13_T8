#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class UAnimDataModel;
class USkeleton;
struct FFrameTime;

class UAnimDataController : public UObject
{
    DECLARE_CLASS(UAnimDataController, UObject)

public:
    UAnimDataController() = default;

    virtual void SetModel(UAnimDataModel* InModel);
    virtual UAnimDataModel* GetModel() const { return Model; }
    virtual void SetNumberOfFrames(int32 Length);
    virtual void ResizeNumberOfFrames(int32 NewLength, int32 T0, int32 T1);
    virtual void SetPlayLength(float Length);
    virtual int32 AddBoneTrack(FName BoneName);
    virtual bool RemoveBoneTrack(FName BoneName);
    virtual void RemoveAllBoneTracks();
    virtual bool SetBoneTrackKeys(FName BoneName, const TArray<FVector>& PositionalKeys, const TArray<FQuat>& RotationalKeys, const TArray<FVector>& ScalingKeys);
    virtual bool RemoveBoneTracksMissingFromSkeleton(const USkeleton* Skeleton);
    virtual void UpdateWithSkeleton(USkeleton* TargetSkeleton);

    int32 ConvertSecondsToFrameNumber(double Seconds) const;

    void SetFrameRate(int32 FrameRate);

private:
    UPROPERTY(ShallowCopy, UAnimDataModel*, Model, = nullptr)
};
