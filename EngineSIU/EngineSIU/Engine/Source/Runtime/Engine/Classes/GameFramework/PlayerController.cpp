#include "PlayerController.h"

#include "Camera/PlayerCameraManager.h"
#include "Engine/Engine.h"
#include "UObject/UObjectIterator.h"
#include "World/World.h"


void APlayerController::PostSpawnInitialize()
{
    AActor::PostSpawnInitialize();

    SetupInputComponent();
    SpawnPlayerCameraManager();
}

void APlayerController::BeginPlay()
{
}

void APlayerController::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);
    
    if (bHasPossessed)
    {
        ProcessInput(DeltaTime);
    }

    if (PlayerCameraManager)
    {
        PlayerCameraManager->UpdateCamera(DeltaTime);
    }

}

void APlayerController::ProcessInput(float DeltaTime) const
{
    if (InputComponent)
    {
        InputComponent->ProcessKeyInput(DeltaTime);
    }
}

void APlayerController::Destroyed()
{
    UnPossess();
}

void APlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnPossess();
}

UInputComponent* APlayerController::GetInputComponent() const
{
    return InputComponent;
}

void APlayerController::SetViewTarget(class AActor* NewViewTarget, struct FViewTargetTransitionParams TransitionParams)
{
    if (PlayerCameraManager)
    {
        PlayerCameraManager->SetViewTarget(NewViewTarget, TransitionParams);
    }
}

void APlayerController::Possess(APawn* InPawn)
{
    Pawn = InPawn;
    bHasPossessed = true;

    if (InputComponent)
    {
        InputComponent->SetPossess();
    }
}

void APlayerController::UnPossess()
{
    if (!bHasPossessed && Pawn == nullptr)
    {
        return;
    }

    Pawn = nullptr;
    bHasPossessed = false;

    if (InputComponent)
    {
        InputComponent->UnPossess();
    }
}

void APlayerController::SetupInputComponent()
{
    // What is the correct parameter of ConstructObject?
    if (InputComponent == nullptr)
    {
        InputComponent = AddComponent<UInputComponent>();
    }
}

void APlayerController::BindAction(const FString& Key, const std::function<void(float)>& Callback)
{
    if (InputComponent)
    {
        InputComponent->BindKeyAction(Key, Callback);
    }
}

uint64 APlayerController::BindLuaAction(const FString& Key, AActor* LuaObj, const std::function<void(float)>& Callback)
{
    if (InputComponent)
    {
        return InputComponent->BindTargetedKeyAction(Key, LuaObj, Callback);
    }
}

void APlayerController::UnBindLuaAction(const FString& Key, uint64 HandleId)
{
    if (InputComponent)
    {
        return InputComponent->UnBindKeyAction(Key, HandleId);
    }
}

AActor* APlayerController::GetViewTarget() const
{
    AActor* CameraManagerViewTarget = PlayerCameraManager ? PlayerCameraManager->GetViewTarget() : nullptr;

    return CameraManagerViewTarget ? CameraManagerViewTarget : const_cast<APlayerController*>(this);
}

void APlayerController::SpawnPlayerCameraManager()
{
    PlayerCameraManager = GetWorld()->SpawnActor<APlayerCameraManager>();

    if (PlayerCameraManager)
    {
        PlayerCameraManager->InitializeFor(this);
    }
}

void APlayerController::ClientStartCameraShake(UClass* Shake)
{
    if (PlayerCameraManager != nullptr)
    {
        PlayerCameraManager->StartCameraShake(Shake);
    }
}

void APlayerController::ClientStopCameraShake(UClass* Shake, bool bImmediately)
{
    if (PlayerCameraManager != nullptr)
    {
        PlayerCameraManager->StopAllInstancesOfCameraShake(Shake, bImmediately);
    }
}

APlayerCameraManager* APlayerController::GetCameraManager() const 
{
    return PlayerCameraManager;
}
