#include "GameMenu.h"

#include "ImGui/imgui.h"

void FGameMenu::Update(float deltaTime)
{
}

void FGameMenu::Render()
{
    if (!isOpenPtr || !(*isOpenPtr)) return;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 size = ImVec2(300, 200);
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

    // 중앙에 오도록 위치 계산
    ImVec2 pos = ImVec2(center.x - size.x * 0.5f, center.y - size.y * 0.5f);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::Begin("Game Menu", nullptr, flags);
    ImGui::Text("=== PAUSED ===");

    if (ImGui::Button("Resume", ImVec2(100, 0)))
    {
        *isOpenPtr = false;
    }
    
    if (ImGui::Button("Options", ImVec2(100, 0)))
    {
        // 옵션 창 열기 로직
    }
    
    if (ImGui::Button("Quit", ImVec2(100, 0)))
    {
        // 게임 종료 로직
    }
    ImGui::End();
}
