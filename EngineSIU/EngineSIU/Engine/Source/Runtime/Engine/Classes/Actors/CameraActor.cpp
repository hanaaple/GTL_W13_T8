#include "CameraActor.h"

#include "Camera/CameraComponent.h"


ACameraActor::ACameraActor()
{
    Camera = FObjectFactory::ConstructObject<UCameraComponent>(this);
}

void ACameraActor::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
    OutProperties.Add(TEXT("CameraMode"), FString::FromInt(static_cast<uint32>(CameraMode)));
}

void ACameraActor::SetProperties(const TMap<FString, FString>& InProperties)
{
    Super::SetProperties(InProperties);
    CameraMode = static_cast<ECameraMode>(FString::ToInt(*InProperties.Find(TEXT("CameraMode"))));
}
