#pragma once
#include "AnimationAsset.h"
#include "AnimTypes.h"
#include "AnimData/AnimDataModel.h"

class UAnimDataController;
class UAnimDataModel;
class USkeletalMeshComponent;

class UAnimSequenceBase : public UAnimationAsset
{
    DECLARE_CLASS(UAnimSequenceBase, UAnimationAsset)

public:
    UAnimSequenceBase();
    virtual ~UAnimSequenceBase() override = default;

    TArray<FAnimNotifyEvent> Notifies;

    TArray<FAnimNotifyTrack> AnimNotifyTracks;

    UPROPERTY(EditAnywhere, float,  RateScale, = 1.0f)

    UPROPERTY(EditAnywhere, bool,  bLoop, = true)

    float GetPlayLength() { return DataModel->GetPlayLength(); }

protected:
    UPROPERTY(ShallowCopy, UAnimDataModel*, DataModel, = nullptr)

    UPROPERTY(ShallowCopy, UAnimDataController*, Controller, = nullptr)
    
public:
    virtual float GetPlayLength() const override;

    UAnimDataModel* GetDataModel() const;

    UAnimDataController& GetController();

    TArray<FAnimNotifyTrack>& GetAnimNotifyTracks() 
    {
        return AnimNotifyTracks;
    }

    bool AddNotifyTrack(const FName& TrackName, int32& OutNewTrackIndex);
    bool RemoveNotifyTrack(int32 TrackIndexToRemove);
    bool RenameNotifyTrack(int32 TrackIndex, const FName& NewTrackName);
    int32 FindNotifyTrackIndex(const FName& TrackName) const;

    bool AddNotifyEvent(int32 TargetTrackIndex, float Time, float Duration, const FName& NotifyName, int32& OutNewNotifyIndex);
    bool RemoveNotifyEvent(int32 NotifyIndexToRemove);
    bool UpdateNotifyEvent(int32 NotifyIndexToUpdate, float NewTime, float NewDuration, int32 NewTrackIndex, const FName& NewNotifyName = NAME_None);
    FAnimNotifyEvent* GetNotifyEvent(int32 NotifyIndex);
    const FAnimNotifyEvent* GetNotifyEvent(int32 NotifyIndex) const;
    void EvaluateAnimNotifies(const TArray<FAnimNotifyEvent>& Notifies, float CurrentTime, float PreviousTime, float DeltaTime, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimAsset, bool bIsLooping);
    
    virtual void SerializeAsset(FArchive& Ar) override;

private:
    void CreateModel();

public:
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
};
