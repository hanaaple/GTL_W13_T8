#include "GameEnd.h"

#include "Engine/Engine.h"
#include "FFaxk/FFaxkGameMode.h"
#include "ImGui/imgui.h"
#include "UObject/Object.h"
#include "World/World.h"

FGameEnd::FGameEnd()
{
}

void FGameEnd::Update(float deltaTime)
{
    if (AFFaxkGameMode* GM = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
    {
        if (IsValid(GM) && GM->GetGameState() == EGameState::Died)
        {
            if (FSlateAppMessageHandler::IsAnyKeyPressed())
            {
                // TODO : Restart나 Retrun To Main 로직
                APlayerController* PC = GM->GetPlayerController();
                PC->GetPawn()->Destroy();
                GM->RequestPlayerRespawn(PC);
                GM->SetGameState(EGameState::Playing);
            }
        }
    }

}

void FGameEnd::Render()
{
    const AFFaxkGameMode* GM = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>();

    if (!IsValid(GM) || GM->GetGameState() != EGameState::Died)
    {
        return;
    }
    
    ImGuiIO&    IO         = ImGui::GetIO();
    ImVec2      ScreenSize = IO.DisplaySize;
    ImDrawList* DrawList    = ImGui::GetForegroundDrawList();

    // 1) 전체를 검은색으로 채우기
    DrawFullScreenColor(DrawList, ScreenSize, IM_COL32(0, 0, 0, 255));

    // 2) "You Die !" 텍스트 (빨간색, 128px) 화면 세로 40% 지점
    ImFont* InGameFont = GEngineLoop.GetGameUIManager()->GetInGameFont64();
    if (InGameFont && InGameFont->IsLoaded())
    {
        DrawCenteredText(DrawList, InGameFont, 128.0f, ScreenSize, "You Die !", IM_COL32(255, 0, 0, 255), 0.4f);
        DrawCenteredText(DrawList, InGameFont, 64.0f, ScreenSize, "Press Any Key To Restart", IM_COL32(255, 255, 255, 255), 0.6f);
    }
}

FGameEnd::~FGameEnd()
{
}
