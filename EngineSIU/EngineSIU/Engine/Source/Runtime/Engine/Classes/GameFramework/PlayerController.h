#pragma once
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h" 
#include "Classes/Components/InputComponent.h"

class APawn;
class APlayerCameraManager;

class APlayerController : public AActor
{
    DECLARE_CLASS(APlayerController, AActor)
    
public:
    APlayerController() = default;
    virtual ~APlayerController() override = default;

    virtual void PostSpawnInitialize() override;

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
    
    void ProcessInput(float DeltaTime) const;

    virtual void Destroyed() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UInputComponent* GetInputComponent() const { return InputComponent; }

    void SetViewTarget(class AActor* NewViewTarget, struct FViewTargetTransitionParams TransitionParams);

    virtual void Possess(APawn* InPawn);

    virtual void UnPossess();
    
    virtual void BindAction(const FString& Key, const std::function<void(float)>& Callback);

    APawn* GetPawn() const { return Pawn; }

    /** Templated version of GetPawn, will return nullptr if cast fails */
    template <typename T>
    T* GetPawn() const
    {
        return Cast<T>(Pawn);
    }

    virtual uint64 BindLuaAction(const FString& Key, AActor* LuaObj, const TFunction<void(float)>& Callback);

    virtual void UnBindLuaAction(const FString& Key, uint64 HandleId);

    // 카메라 관련 함수
    AActor* GetViewTarget() const;

    virtual void SpawnPlayerCameraManager();

    void ClientStartCameraShake(UClass* Shake);

    void ClientStopCameraShake(UClass* Shake, bool bImmediately = true);

    APlayerCameraManager* PlayerCameraManager = nullptr;
    
protected:
    UPROPERTY(UInputComponent*, InputComponent, = nullptr)

    virtual void SetupInputComponent();

    APawn* Pawn = nullptr;

    bool bHasPossessed = false;
};

