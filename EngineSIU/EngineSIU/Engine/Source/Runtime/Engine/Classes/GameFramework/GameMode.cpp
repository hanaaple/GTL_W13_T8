#include "GameMode.h"
#include "LuaScripts/LuaScriptComponent.h"
#include "EngineLoop.h"
#include "SoundManager.h"
#include "InputCore/InputCoreTypes.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/World/World.h"

AGameMode::AGameMode()
{
    OnGameInit.AddLambda([]() { UE_LOG(ELogLevel::Display, TEXT("Game Initialized")); });
    
    //LuaScriptComp->GetOuter()->

    SetActorTickInEditor(false); // PIE 모드에서만 Tick 수행

    if (FSlateAppMessageHandler* Handler = GEngineLoop.GetAppMessageHandler())
    {
        /*Handler->OnPIEModeStartDelegate.AddLambda([this]()
        {
            this->InitGame();
        });*/
        Handler->OnKeyDownDelegate.AddLambda([this](const FKeyEvent& KeyEvent)
        {
            // 키가 Space, 아직 게임이 안 시작됐고, 실패 또는 종료되지 않았다면
            if (KeyEvent.GetKeyCode() == VK_SPACE &&
                !bGameRunning && bGameEnded)
            {
                StartMatch();
            }
        });

        Handler->OnKeyDownDelegate.AddLambda([this](const FKeyEvent& KeyEvent)
            {
                // 키가 Space, 아직 게임이 안 시작됐고, 실패 또는 종료되지 않았다면
                if (KeyEvent.GetKeyCode() == VK_RCONTROL && 
                    bGameRunning && !bGameEnded)
                {
                    EndMatch(false);
                }
            });
    }
}



AGameMode::~AGameMode()
{
    // EndMatch(false);
}

void AGameMode::InitializeComponent()
{
    //ULuaScriptComponent* LuaScriptComp = this->AddComponent<ULuaScriptComponent>();
    //RootComponent = this->AddComponent<USceneComponent>("USceneComponent_0");
}

UObject* AGameMode::Duplicate(UObject* InOuter)
{
    AGameMode* NewActor = Cast<AGameMode>(Super::Duplicate(InOuter));

    if (NewActor)
    {
        NewActor->bGameRunning = bGameRunning;
        NewActor->bGameEnded = bGameEnded;
        NewActor->GameInfo = GameInfo;
    }
    return NewActor;
}


void AGameMode::InitGame()
{
    Super::InitGame();

    OnGameInit.Broadcast();
}

void AGameMode::StartMatch()
{
    bGameRunning = true;
    bGameEnded = false;
    GameInfo.ElapsedGameTime = 0.0f;
    GameInfo.TotalGameTime = 0.0f;
    
    OnGameStart.Broadcast();
}

void AGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bGameRunning && !bGameEnded)
    {
        GameInfo.ElapsedGameTime += DeltaTime / 2.0f;
    }
}

void AGameMode::EndMatch(bool bIsWin)
{
    // 이미 종료된 상태라면 무시
    if (!bGameRunning || bGameEnded)
    {
        return;
    }

    this->Reset();
    
    GameInfo.TotalGameTime = GameInfo.ElapsedGameTime;
    
    OnGameEnd.Broadcast(bIsWin);
}

void AGameMode::Reset()
{
    bGameRunning = false;
    bGameEnded = true;
}
