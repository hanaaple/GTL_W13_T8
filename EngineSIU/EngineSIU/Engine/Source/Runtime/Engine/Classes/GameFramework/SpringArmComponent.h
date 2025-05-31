#pragma once

#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "Math/Quat.h"
#include "Math/Vector.h"
#include "PropertyEditor/PropertyEditorPanel.h"


class USpringArmComponent : public USceneComponent
{
    DECLARE_CLASS(USpringArmComponent, USceneComponent)
    friend class PropertyEditorPanel;
public:
    USpringArmComponent();
    virtual ~USpringArmComponent() override = default;

    virtual void TickComponent(float DeltaTime) override;

    void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);

    FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);

    static bool RaySweepBox(const FVector& Start, const FVector& Dir, float MaxDist, const FMatrix& BoxMatrix,
                            const FVector& BoxExtents, float ProbeRadius, float& OutT);

public:
    FRotator GetDesiredRotation() const;
    FRotator GetTargetRotation() const;
private:
    UPROPERTY(EditAnywhere, FVector, TargetOffset, = FVector(-13.f, 0.f, 4.f))// 월드 공간 오프셋
    UPROPERTY(EditAnywhere, float, TargetArmLength, = 5.0f) // 카메라와의 거리

    UPROPERTY(EditAnywhere, float, ProbeSize, = 0.3f)// 충돌에 사용할 구 반지름

    UPROPERTY(EditAnywhere, uint32, bDoCollisionTest, = false)// 암 길이 자동으로 줄여 충돌을 방지할지 여부
    UPROPERTY(EditAnywhere, uint32, bUsePawnControlRotation, = true)// 부모의 회전값을 사용할지 여부

    UPROPERTY(EditAnywhere, uint32, bInheritPitch, = true)
    UPROPERTY(EditAnywhere, uint32, bInheritYaw, = true)
    UPROPERTY(EditAnywhere, uint32, bInheritRoll, = true)

    UPROPERTY(EditAnywhere, uint32, bEnableCameraLag, = true)// 위치 지연 여부
    UPROPERTY(EditAnywhere, uint32, bEnableCameraRotationLag, = true)// 회전 지연 여부
    UPROPERTY(EditAnywhere, uint32, bUseCameraLagSubstepping, = true)// sub-stepping으로 작은 조각으로 보간

    UPROPERTY(EditAnywhere, float, CameraLagSpeed, = 10.f)// 위치 지연 속도
    UPROPERTY(EditAnywhere, float, CameraRotationLagSpeed, = 10.f)// 회전 지연 속도
    UPROPERTY(EditAnywhere, float, CameraLagMaxTimeStep, = 1.f / 60.f)// sub-stepping의 마지노선 
    UPROPERTY(EditAnywhere, float, CameraLagMaxDistance, = 0.f)// 목표 위치로부터 지연 위치의 최대 상한


    /* 이전 프레임의 위치 회전 등 저장할 임시 변수 */
    FVector PreviousDesiredLoc;
    FVector PreviousArmOrigin;
    FRotator PreviousDesiredRot;
    
    FVector RelativeSocketLocation; // 암을 따라 회전되는 로컬 오프셋
    FQuat RelativeSocketRotation;
};
