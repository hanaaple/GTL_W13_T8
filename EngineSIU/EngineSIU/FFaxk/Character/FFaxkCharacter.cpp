#include "FFaxkCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// #include "LuaScripts/LuaScriptComponent.h"


AFFaxkCharacter::AFFaxkCharacter()
{
    // LuaComponent->SetScriptPath("");
    // GetMesh()

    SpringArm = AddComponent<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArm->bEnableCameraLag = false;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = AddComponent<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->FarClip = 25000.0f;
    CameraComponent->SetupAttachment(SpringArm);
}

void AFFaxkCharacter::BeginPlay()
{
    ACharacter::BeginPlay();
}

void AFFaxkCharacter::Tick(float DeltaTime)
{
    ACharacter::Tick(DeltaTime);
}
