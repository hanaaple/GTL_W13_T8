#pragma once
#include "ActorComponent.h"
#include "Math/Rotator.h"
#include "UObject/ObjectMacros.h"
#include "Math/Transform.h"

struct FHitResult;
struct FOverlapInfo;

class USceneComponent : public UActorComponent
{
    DECLARE_CLASS(USceneComponent, UActorComponent)

public:
    USceneComponent();

    void GetProperties(TMap<FString, FString>& OutProperties) const override;
    void SetProperties(const TMap<FString, FString>& InProperties) override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual void EndPhysicsTickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(const FVector& InRayOrigin, const FVector& InRayDirection, float& OutHitDistance) const;
    virtual void DestroyComponent(bool bPromoteChildren = false) override;

    UFUNCTION_CONST(FVector, GetForwardVector)
    UFUNCTION_CONST(FVector, GetRightVector)
    UFUNCTION_CONST(FVector, GetUpVector)
    
    UFUNCTION(void, AddLocation, const FVector& InAddValue)
    UFUNCTION(void, AddRotation, const FRotator& InAddValue)
    UFUNCTION(void, AddScale, const FVector& InAddValue)
    void AddRotation(const FQuat& InAddValue);

    USceneComponent* GetAttachParent() const { return AttachParent; }
    const TArray<USceneComponent*>& GetAttachChildren() const { return AttachChildren; }

    void AttachToComponent(USceneComponent* InParent);
    void SetupAttachment(USceneComponent* InParent);
    void DetachFromComponent(USceneComponent* Target);
    
public:
    UFUNCTION(void, SetRelativeLocation, const FVector& InLocation);
    UFUNCTION(void, SetRelativeRotation, const FRotator& InRotation);
    UFUNCTION(void, SetRelativeScale3D, const FVector& InScale);
    void SetRelativeRotation(const FQuat& InQuat);
    void SetRelativeTransform(const FTransform& InTransform);
    
    UFUNCTION_CONST(FVector, GetRelativeLocation)
    UFUNCTION_CONST(FRotator, GetRelativeRotation)
    UFUNCTION_CONST(FVector, GetRelativeScale3D)
    FTransform GetRelativeTransform() const;

    UFUNCTION(void, SetWorldLocation, const FVector& InLocation);
    UFUNCTION(void, SetWorldRotation, const FRotator& InRotation);
    UFUNCTION(void, SetWorldScale3D, const FVector& InScale);
    void SetWorldRotation(const FQuat& InQuat);
    void SetWorldTransform(const FTransform& InTransform);
    
    UFUNCTION_CONST(FVector, GetComponentLocation)
    UFUNCTION_CONST(FRotator, GetComponentRotation)
    UFUNCTION_CONST(FVector, GetComponentScale3D)
    FTransform GetComponentTransform() const;

    FMatrix GetScaleMatrix() const;
    FMatrix GetRotationMatrix() const;
    FMatrix GetTranslationMatrix() const;

    FMatrix GetWorldMatrix() const;

    void UpdateOverlaps(const TArray<FOverlapInfo>* PendingOverlaps = nullptr, bool bDoNotifies = true, const TArray<const FOverlapInfo>* OverlapsAtEndLocation = nullptr);

    bool MoveComponent(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = nullptr);
    bool MoveComponent(const FVector& Delta, const FRotator& NewRotation, bool bSweep, FHitResult* OutHit = nullptr);

    FTransform GetComponentToWorld() const { return ComponentToWorld; }

protected:
    /** 부모 컴포넌트로부터 상대적인 위치 */
    UPROPERTY
    (FVector, RelativeLocation)

    /** 부모 컴포넌트로부터 상대적인 회전 */
    UPROPERTY
    (FRotator, RelativeRotation)

    /** 부모 컴포넌트로부터 상대적인 크기 */
    UPROPERTY
    (FVector, RelativeScale3D)

    UPROPERTY
    (USceneComponent*, AttachParent, = nullptr)

    UPROPERTY
    (TArray<USceneComponent*>, AttachChildren)

    virtual void UpdateOverlapsImpl(const TArray<FOverlapInfo>* PendingOverlaps = nullptr, bool bDoNotifies = true, const TArray<const FOverlapInfo>* OverlapsAtEndLocation = nullptr);

    virtual bool MoveComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = nullptr);

public:
    bool IsUsingAbsoluteRotation() const;
    void SetUsingAbsoluteRotation(const bool bInAbsoluteRotation);
protected:
    uint8 bAbsoluteRotation : 1;
    
private:
    // TODO: 캐싱해서 사용하기
    bool bComponentToWorldUpdated = true;

    UPROPERTY(FTransform, ComponentToWorld, = FTransform::Identity)
};
