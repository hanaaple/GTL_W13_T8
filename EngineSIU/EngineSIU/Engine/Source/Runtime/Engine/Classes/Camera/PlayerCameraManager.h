#pragma once
#include "CameraTypes.h"
#include "GameFramework/Actor.h"

class UCameraShakeBase;
class UCameraComponent;
class UCameraModifier_CameraShake;
class APlayerController;
class UCameraModifier;

DECLARE_MULTICAST_DELEGATE(FOnBlendComplete)

enum EViewTargetBlendOrder : int
{
    VTBlendOrder_Base,
    VTBlendOrder_Override
};

enum EViewTargetBlendFunction : int
{
    /** Camera does a simple linear interpolation. */
    VTBlend_Linear,
    /** Camera has a slight ease in and ease out, but amount of ease cannot be tweaked. */
    VTBlend_Cubic,
    /** Camera immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by BlendExp. */
    VTBlend_EaseIn,
    /** Camera smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by BlendExp. */
    VTBlend_EaseOut,
    /** Camera smoothly accelerates and decelerates.  Ease amount controlled by BlendExp. */
    VTBlend_EaseInOut,
    /** The game's camera system has already performed the blending. Engine should not blend at all */
    VTBlend_PreBlended,
    VTBlend_MAX,
};

struct FTViewTarget
{
    AActor* Target; 
    FMinimalViewInfo POV;

public:
    bool Equal(const FTViewTarget& OtherTarget) const;

    FTViewTarget()
        : Target(nullptr)
    {
    }

    void CheckViewTarget(APlayerController* OwningController);

    void SetNewTarget(AActor* NewTarget);
    AActor* GetTargetActor() const;
};

struct FViewTargetTransitionParams
{
public:
    float BlendTime;

    EViewTargetBlendFunction BlendFunction;

    float BlendExp;

    uint32 bLockOutgoing:1;

    FViewTargetTransitionParams()
        : BlendTime(0.f)
        , BlendFunction(VTBlend_Cubic)
        , BlendExp(2.f)
        , bLockOutgoing(false)
    {}

    /** For a given linear blend value (blend percentage), return the final blend alpha with the requested function applied */
    float GetBlendAlpha(const float& TimePct) const
    {
        switch (BlendFunction)
        {
        case VTBlend_Linear:
            return FMath::Lerp(0.f, 1.f, TimePct); 
        case VTBlend_Cubic:
            return FMath::CubicInterp(0.f, 0.f, 1.f, 0.f, TimePct); 
        case VTBlend_EaseInOut:
            return FMath::InterpEaseInOut(0.f, 1.f, TimePct, BlendExp); 
        case VTBlend_EaseIn:
            return FMath::Lerp(0.f, 1.f, FMath::Pow(TimePct, BlendExp)); 
        case VTBlend_EaseOut:
            const float Exp = FMath::IsNearlyZero(BlendExp) ? 1.f : (1.f / BlendExp);
            return FMath::Lerp(0.f, 1.f, FMath::Pow(TimePct, Exp));
        }

        return 1.f;
    }
};

class APlayerCameraManager : public AActor
{
    DECLARE_CLASS(APlayerCameraManager, AActor)
    
    APlayerController* PCOwner;
    
public:
    APlayerCameraManager();

    virtual void PostSpawnInitialize() override;

    virtual void InitializeFor(APlayerController* PC);

    AActor* GetViewTarget() const;
    
    void UpdateCamera(float DeltaTime);

    /* Fade IN / OUT */
    void StartCameraFade(float FromAlpha, float ToAlpha, float Duration, FLinearColor Color, bool bHoldWhenFinished = false);

    void StopCameraFade();

    void ApplyCameraModifiers(float DeltaTime, FMinimalViewInfo& InOutPOV);

    void AssignViewTarget(AActor* NewTarget, FTViewTarget& VT, struct FViewTargetTransitionParams TransitionParams=FViewTargetTransitionParams());

    void StartVignetteAnimation(float FromIntensity, float ToIntensity, float Duration);

    void SetViewTarget(class AActor* NewTarget, struct FViewTargetTransitionParams TransitionParams);

    UFUNCTION(void, SetViewTarget, AActor* NewTarget);

    virtual UCameraShakeBase* StartCameraShake(UClass* ShakeClass);

    virtual void StopCameraShake(UCameraShakeBase* ShakeInstance, bool bImmediately = true);

    virtual void StopAllInstancesOfCameraShake(UClass* ShakeClass, bool bImmediately = true);

    float GetLetterBoxRatio();
    
protected:
    virtual void DoUpdateCamera(float DeltaTime);

    virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime);
    
    void SetCameraVignette(float InIntensity, float InRadius, float InSmoothness);

    void SetCameraVignetteColor(FLinearColor InColor);
    
    FMinimalViewInfo BlendViewTargets(const FTViewTarget& A, const FTViewTarget& B, float Alpha);

    FMinimalViewInfo LastFrameFOV;

    TArray<UCameraModifier*> ModifierList;

    UCameraModifier_CameraShake* CachedCameraShakeMod;
    
public:
    FTViewTarget ViewTarget;

    FTViewTarget PendingViewTarget;

    FTViewTarget LastFrameViewTarget;

    float BlendTimeToGo;

    FViewTargetTransitionParams BlendParams;
    
    FLinearColor FadeColor;

    float FadeAmount;

    FVector2D FadeAlpha;

    float FadeTime;

    float FadeTimeRemaining;

    FName CameraStyle;

    float DefaultFOV;
    float DefaultAspectRatio;
    uint32 bDefaultConstrainAspectRatio : 1;

    mutable FOnBlendComplete OnBlendCompleteEvent;

  	/** Minimum view pitch, in degrees. */
    float ViewPitchMin;
    /** Maximum view pitch, in degrees. */
    float ViewPitchMax;
    /** Minimum view yaw, in degrees. */
    float ViewYawMin;
    /** Maximum view yaw, in degrees. */
    float ViewYawMax;
    /** Minimum view roll, in degrees. */
    float ViewRollMin;
    /** Maximum view roll, in degrees. */
    float ViewRollMax;
    
    // [TEMP] Vignette factor
    FVector2D VignetteCenter;

    FLinearColor VignetteColor;

    float VignetteRadius;

    float VignetteIntensity;

    float VignetteSmoothness;

    float VignetteTime;

    float VignetteTimeRemaining;

    float VignetteStartIntensity;

    float VignetteTargetIntensity;

    // [TEMP] LetterBox factor;
    float LetterBoxWidth;

    float LetterBoxHeight;

    uint32 bEnableFading : 1;
    uint32 bAnimateVignette : 1;
    uint32 bHoldFadeWhenFinished : 1; /* true일 경우 페이드 종료 상태 유지 */

    // Depth of Field
    float F_Stop = 2.8f;
    float SensorWidth = 24.576f; // mm
    float FocalDistance = 0.f; // cm

    float GetFocalLength() const;
};

