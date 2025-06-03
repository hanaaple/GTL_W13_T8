#pragma once
#include "SkinnedMeshComponent.h"
#include "Actors/Player.h"
#include "Engine/AssetManager.h"
#include "Engine/Asset/SkeletalMeshAsset.h"
#include "Template/SubclassOf.h"
#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Engine/Contents/AnimInstance/MyAnimInstance.h"

struct FConstraintInstance;
class UAnimSequence;
class USkeletalMesh;
class FAnimNotifyEvent;
class UAnimSequenceBase;

enum class EAnimationMode : uint8
{
    AnimationBlueprint,
    AnimationSingleNode,
};

class USkeletalMeshComponent : public USkinnedMeshComponent
{
    DECLARE_CLASS(USkeletalMeshComponent, USkinnedMeshComponent)

public:
    USkeletalMeshComponent();
    virtual ~USkeletalMeshComponent() override = default;

    virtual void InitializeComponent() override;

    //virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;

    virtual void TickComponent(float DeltaTime) override;
    virtual void EndPhysicsTickComponent(float DeltaTime) override;

    virtual void TickPose(float DeltaTime) override;

    virtual void CreatePhysXGameObject() override;
    virtual void DestroyPhysXGameObject() override;

protected:
    virtual void UpdatePhysXGameObject() override;
    
public:

    void TickAnimation(float DeltaTime);

    void TickAnimInstances(float DeltaTime);

    bool ShouldTickAnimation() const;

    bool InitializeAnimScriptInstance();

    void ClearAnimScriptInstance();

    USkeletalMesh* GetSkeletalMeshAsset() const { return SkeletalMeshAsset; }

    void SetSkeletalMeshAsset(USkeletalMesh* InSkeletalMeshAsset);

    FTransform GetSocketTransform(FName SocketName) const;

    TArray<FTransform> RefBonePoseTransforms; // 원본 BindPose에서 복사해온 에디팅을 위한 Transform

    void GetCurrentGlobalBoneMatrices(TArray<FMatrix>& OutBoneMatrices) const;

    void DEBUG_SetAnimationEnabled(bool bEnable);

    void PlayAnimation(UAnimationAsset* NewAnimToPlay, bool bLooping);

    void SetAnimation(UAnimationAsset* NewAnimToPlay);

    UAnimationAsset* GetAnimation() const;

    void Play(bool bLooping);

    void Stop();

    void SetPlaying(bool bPlaying);
    
    bool IsPlaying() const;

    void SetReverse(bool bIsReverse);
    
    bool IsReverse() const;

    void SetPlayRate(float Rate);

    float GetPlayRate() const;

    void SetLooping(bool bIsLooping);

    bool IsLooping() const;

    int GetCurrentKey() const;

    void SetCurrentKey(int InKey);

    void SetElapsedTime(float InElapsedTime);

    float GetElapsedTime() const;

    int32 GetLoopStartFrame() const;

    void SetLoopStartFrame(int32 InLoopStartFrame);

    int32 GetLoopEndFrame() const;

    void SetLoopEndFrame(int32 InLoopEndFrame);
    
    bool bIsAnimationEnabled() const { return bPlayAnimation; }
    
    virtual int CheckRayIntersection(const FVector& InRayOrigin, const FVector& InRayDirection, float& OutHitDistance) const override;

    const FSkeletalMeshRenderData* GetCPURenderData() const;

    static void SetCPUSkinning(bool Flag);

    static bool GetCPUSkinning();

    UAnimInstance* GetAnimInstance() const { return AnimScriptInstance; }

    void SetAnimationMode(EAnimationMode InAnimationMode);

    EAnimationMode GetAnimationMode() const { return AnimationMode; }

    virtual void InitAnim();

protected:
    bool NeedToSpawnAnimScriptInstance() const;

    UPROPERTY(EditAnywhere, EAnimationMode, AnimationMode, = EAnimationMode::AnimationSingleNode)
    
private:
    FPoseContext BonePoseContext;

    UPROPERTY(EditAnywhere | ShallowCopy, USkeletalMesh*, SkeletalMeshAsset, = nullptr)

    bool bPlayAnimation;

    std::unique_ptr<FSkeletalMeshRenderData> CPURenderData = nullptr;

    static bool bIsCPUSkinning;

    /** Array of FBodyInstance objects, storing per-instance state about about each body. */
    TArray<FBodyInstance*> Bodies;

    /** Array of FConstraintInstance structs, storing per-instance state about each constraint. */
    TArray<FConstraintInstance*> Constraints;

    void CPUSkinning(bool bForceUpdate = false);

public:
    TSubclassOf<UAnimInstance> AnimClass;

    UPROPERTY(EditAnywhere, UAnimInstance*, AnimScriptInstance, = nullptr)

    UFUNCTION(UAnimInstance*, GetAnimInstance);

    UFUNCTION(UMyAnimInstance*, GetMyAnimInstance);
    
    UFUNCTION_CONST(UAnimSingleNodeInstance*, GetSingleNodeInstance);

    void SetAnimClass(UClass* NewClass);
    
    UClass* GetAnimClass() const;
    
    void SetAnimInstanceClass(class UClass* NewClass);
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
    virtual void PostDuplicate() override;
};
