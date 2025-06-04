#include "AnimSoundNotify.h"
#include "SoundManager.h"

UAnimSoundNotify::UAnimSoundNotify()
{
}

void UAnimSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    FSoundManager::GetInstance().PlaySound(*SoundName.ToString());
}

