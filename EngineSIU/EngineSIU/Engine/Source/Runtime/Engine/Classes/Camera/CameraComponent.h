#pragma once
#include "CameraTypes.h"
#include "Components/SceneComponent.h"

class UCameraComponent : public USceneComponent
{
public:
    DECLARE_CLASS(UCameraComponent, USceneComponent)

    UCameraComponent() = default;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    void FollowMainPlayer();

    UPROPERTY(EditAnywhere, float, ViewFOV, = 70.0f)
    UPROPERTY(EditAnywhere, float, NearClip, = 0.1f)
    UPROPERTY(EditAnywhere, float, FarClip, = 1000.0f)

    UPROPERTY(EditAnywhere, float, DistanceBehind, = 10.f)
    UPROPERTY(EditAnywhere, float, CameraHeight, =  15.f)
    UPROPERTY(EditAnywhere, float, CurrentCameraZ, =  0.f)
    UPROPERTY(EditAnywhere, float, CameraZ, =0.f)
    UPROPERTY(EditAnywhere, float, CameraZOffset, = 8.f)

    void SetLocationWithFInterpTo(const FVector& ToLocation);
    void SetFInterpToSpeed(float InSpeed);
    void SetLookTarget(FVector& Location);

    FVector GetLocationWithFInterpTo() const { return FInterpTargetLocation; }

    UPROPERTY(EditAnywhere, bool, bFollowCustomTarget, = false)
    UPROPERTY(EditAnywhere, FVector, CustomTargetLocation, = FVector::ZeroVector)

    void SetFollowCustomTarget(const FVector& InLocation);
    void ResetFollowToPlayer();
    void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView);

private:
    void ProceedFInterp(float DeltaTime);

    UPROPERTY(EditAnywhere, FVector, FInterpTargetLocation, = FVector::ZeroVector)
    UPROPERTY(EditAnywhere, FVector, LookTarget, = FVector::ZeroVector)
    UPROPERTY(EditAnywhere, float, FInterpToSpeed, = 0.8f)
};
