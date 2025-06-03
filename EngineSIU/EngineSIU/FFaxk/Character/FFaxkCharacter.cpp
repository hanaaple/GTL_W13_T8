#include "FFaxkCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "FFaxk/FFaxkGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "LuaScripts/LuaScriptComponent.h"
#include "World/World.h"

AFFaxkCharacter::AFFaxkCharacter()
{
    LuaComponent = AddComponent<ULuaScriptComponent>();
    std::filesystem::path GamePath = std::filesystem::absolute("LuaScripts/inputtest.lua");
    
    LuaComponent->SetScriptPath(GamePath.generic_string());
    GetMesh()->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh("Contents/Character/Paladin_J_Nordstrom"));
    
    SpringArm = AddComponent<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArm->bEnableCameraLag = false;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->TargetOffset = FVector(-15.0f, 0.0f, 150.0f);
    SpringArm->TargetArmLength = 200.0f;
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

    if (GetActorLocation().Z < 0.0f || GetActorLocation().Z > 2500.0f)
    {
        if (GEngine->ActiveWorld)
        {
            if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
            {
                if (GameMode->GetGameState() == EGameState::Playing)
                {
                    Die();
                }
            }
        }
    }
}

void AFFaxkCharacter::Die()
{
    if (GEngine->ActiveWorld)
    {
        if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
        {
            GameMode->SetGameState(EGameState::Died);
            if (APlayerCameraManager* PCM = GameMode->GetPlayerController()->GetCameraManager())
            {
                PCM->StartCameraFade(0.0f, 1.0f, 3.0f, FLinearColor::Black, true);
                PCM->OnFadeCompleteEvent.AddWeakLambda(this, [&]()
                {
                    if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
                    {
                        GameMode->SetGameState(EGameState::End);
                    }
                });
            }
        }
    }
}
