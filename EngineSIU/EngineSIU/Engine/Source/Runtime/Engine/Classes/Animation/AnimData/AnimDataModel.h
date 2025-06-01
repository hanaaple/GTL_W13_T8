
#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

struct FFrameTime;
enum class EAnimInterpolationType : uint8;
class UAnimSequence;
struct FBoneAnimationTrack;
class USkeleton;
struct FTransform;
class UAnimDataController;

class UAnimDataModel : public UObject
{
    DECLARE_CLASS(UAnimDataModel, UObject)
    
public:
    UAnimDataModel();
    virtual ~UAnimDataModel() override = default;

    virtual const TArray<FBoneAnimationTrack>& GetBoneAnimationTracks() const;
    virtual const FBoneAnimationTrack& GetBoneTrackByIndex(int32 TrackIndex) const;
    virtual const FBoneAnimationTrack& GetBoneTrackByName(const FName& TrackName) const;
    virtual const FBoneAnimationTrack* FindBoneTrackByName(FName Name) const;
    virtual const FBoneAnimationTrack* FindBoneTrackByIndex(int32 BoneIndex) const;
    virtual int32 GetBoneTrackIndex(const FBoneAnimationTrack& Track) const;
    virtual int32 GetBoneTrackIndexByName(FName TrackName) const;
    virtual bool IsValidBoneTrackIndex(int32 TrackIndex) const;

    // Begin IAnimationDataModel
    virtual FTransform EvaluateBoneTrackTransform(FName TrackName, const FFrameTime& FrameTime, const EAnimInterpolationType& Interpolation) const;
    virtual FTransform GetBoneTrackTransform(FName TrackName, const int32& FrameNumber) const;
    virtual void GetBoneTrackTransforms(FName TrackName, const TArray<int32>& FrameNumbers, TArray<FTransform>& OutTransforms) const;
    virtual void GetBoneTrackTransforms(FName TrackName, TArray<FTransform>& OutTransforms) const;
    virtual void GetBoneTracksTransform(const TArray<FName>& TrackNames, const int32& FrameNumber, TArray<FTransform>& OutTransforms) const;

    virtual int32 GetNumBoneTracks() const;
    virtual void GetBoneTrackNames(TArray<FName>& OutNames) const;
    
    virtual double GetPlayLength() const;
    virtual int32 GetNumberOfFrames() const;
    virtual int32 GetNumberOfKeys() const;
    virtual int32 GetFrameRate() const;

    virtual UAnimSequence* GetAnimationSequence() const;

    virtual UAnimDataController* GetController();
    // End IAnimationDataModel
    
    USkeleton* GetSkeleton() const;

private:
    // All individual bone animation tracks
    TArray<FBoneAnimationTrack> BoneAnimationTracks;

    // Rate at which the animated data is sampled
    UPROPERTY(int32, FrameRate, = 0)

    // Total number of sampled animated frames
    UPROPERTY(int32, NumberOfFrames, = 0)

    // Total number of sampled animated keys
    UPROPERTY(int32, NumberOfKeys, = 0)

    FBoneAnimationTrack* FindMutableBoneTrackByName(FName Name);

public:
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

private:
    friend class UAnimDataController;
};
