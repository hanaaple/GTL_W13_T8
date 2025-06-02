#include "GameMenu.h"

#include "EngineLoop.h"
#include "DirectXTK/PostProcess.h"
#include "ImGui/imgui.h"

void FGameMenu::Update(float deltaTime)
{
}

void FGameMenu::Render()
{
    if (!FEngineLoop::PauseRequestedPtr)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screen = io.DisplaySize;
    
    // 1) 풀스크린 반투명 회색 배경
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove       |
        ImGuiWindowFlags_NoResize     |
        ImGuiWindowFlags_NoBackground;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(screen, ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##PauseOverlay", nullptr, flags);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    // 회색(50,50,50)에 알파 192 정도로 반투명 처리
    drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screen.x, screen.y), IM_COL32(50, 50, 50, 192));

    ImGui::PopStyleVar(); // WindowPadding
    
    // 2) 버튼 크기 및 간격 설정
    const ImVec2 btnSize(200.0f, 40.0f);
    const float   spacing = 20.0f;
    
    // 3) 세로 위치 시작점: 화면 높이의 30% 지점
    float startY = screen.y * 0.3f;
    // 4) 가로 중앙 정렬: (화면 너비 - 버튼 너비) / 2
    float posX = (screen.x - btnSize.x) * 0.5f;

    // Resume 버튼
    ImGui::SetCursorScreenPos(ImVec2(posX, startY));
    if (ImGui::Button("Resume", btnSize))
    {
        FEngineLoop::PauseRequestedPtr = false;
    }

    // ReStart 버튼
    ImGui::SetCursorScreenPos(ImVec2(posX, startY + btnSize.y + spacing));
    if (ImGui::Button("ReStart", btnSize))
    {
        
    }

    // Quit 버튼
    ImGui::SetCursorScreenPos(ImVec2(posX, startY + 2 * (btnSize.y + spacing)));
    if (ImGui::Button("Quit Game", btnSize))
    {
        PostQuitMessage(0); // Win32 메시지 루프에 WM_QUIT 전송
    }

    ImGui::End();
}

FGameMenu::~FGameMenu()
{
}
