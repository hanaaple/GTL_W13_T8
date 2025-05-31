#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Camera/CameraTypes.h"

class APlayerCameraManager;

class UCameraModifier : public UObject
{
    DECLARE_CLASS(UCameraModifier, UObject)

public:
    UCameraModifier();

    /* 카메라에 적용할 변형/수정 효과를 매 프레임마다 적용
     * 위치/회전/FOV/PostProcess 등에 영향
     * @return bool True if should STOP looping the chain, false otherwise
     */
    virtual bool ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV); 

public:
    UWorld* GetWorld() const;
    bool IsDisabled() const { return bDisabled; }

protected:
    void EnableModifier();
    void DisableModifier(bool bImmediate);
    /** @return Returns the ideal blend alpha for this modifier. Interpolation will seek this value. */
    virtual float GetTargetAlpha();
    void UpdateAlpha(float DeltaTime);


    APlayerCameraManager* CameraOwner;

protected:
    UPROPERTY(EditAnywhere, uint32, bDisabled, = false)
    UPROPERTY(EditAnywhere, uint32, bPendingDisable, = false)

    UPROPERTY(EditAnywhere, uint8, Priority, = 0)

    UPROPERTY(EditAnywhere, float, AlphaInTime, = 0.0f)
    UPROPERTY(EditAnywhere, uint8, AlphaOutTime, = 0.0f)
    UPROPERTY(EditAnywhere, uint8, Alpha, = 0.0f)
};

