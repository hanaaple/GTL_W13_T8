#pragma once
#include "Camera/CameraShakeBase.h"
#include "UObject/ObjectMacros.h"

class USimpleCameraShakePattern : public UCameraShakePattern
{
    DECLARE_CLASS(USimpleCameraShakePattern, UCameraShakePattern)

public:
    USimpleCameraShakePattern() = default;
    
    /** Duration in seconds of this shake. Zero or less means infinite. */
    UPROPERTY(EditAnywhere, float, Duration, = 1.f)

    /** Blend-in time for this shake. Zero or less means no blend-in. */
    UPROPERTY(EditAnywhere, float, BlendInTime, = 0.2f)

    /** Blend-out time for this shake. Zero or less means no blend-out. */
    UPROPERTY(EditAnywhere, float, BlendOutTime, = 0.2f)

protected:

    // UCameraShakePattern interface
    virtual void GetShakePatternInfoImpl(FCameraShakeInfo& OutInfo) const override;
    virtual void StartShakePatternImpl() override;
    virtual bool IsFinishedImpl() const override;
    virtual void StopShakePatternImpl(bool bImmediately) override;
    virtual void TeardownShakePatternImpl()  override;

public:
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

protected:

    /** The ongoing state for this shake */
    FCameraShakeState State;
};
