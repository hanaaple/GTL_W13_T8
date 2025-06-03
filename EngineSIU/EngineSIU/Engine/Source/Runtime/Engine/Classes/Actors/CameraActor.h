#pragma once
#include "GameFramework/Actor.h"
class UCameraComponent;

enum class ECameraMode
{
    Start,
    Ending
};

class ACameraActor : public AActor
{
    DECLARE_CLASS(ACameraActor, AActor)

public:
    ACameraActor();
    virtual ~ACameraActor() override = default;

    [[nodiscard]] ECameraMode GetCameraMode() const
    {
        return CameraMode;
    }

    void SetCameraMode(ECameraMode InCameraMode)
    {
        CameraMode = InCameraMode;
    }

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

private:
    UPROPERTY(EditAnywhere, ECameraMode, CameraMode, = ECameraMode::Start)

    UPROPERTY(EditAnywhere, UCameraComponent*, Camera, = nullptr)
};
