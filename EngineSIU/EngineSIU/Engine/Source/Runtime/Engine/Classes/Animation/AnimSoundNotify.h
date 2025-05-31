#pragma once
#include "AnimNotify.h"

class UAnimSoundNotify : public UAnimNotify
{
public:
    DECLARE_CLASS(UAnimSoundNotify, UAnimNotify)

    UAnimSoundNotify();
    virtual ~UAnimSoundNotify() override = default;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
    virtual FName GetNotifyName() const override { return FName("AnimSoundNotify"); }

    public:
    FName GetSoundName() const { return SoundName; }
    void SetSoundName(FName InSoundName) { SoundName = InSoundName; }
private:
    UPROPERTY(EditAnywhere, FName, SoundName, = NAME_None)
};
