#include "GameHUD.h"

#include "EngineLoop.h"
#include "ImGui/imgui.h"

FGameHUD::FGameHUD()
{
}

FGameHUD::~FGameHUD()
{
}

void FGameHUD::Update(float deltaTime)
{
}

void FGameHUD::Render()
{
    ImGuiIO& IO      = ImGui::GetIO();
    ImVec2   ScreenSize  = IO.DisplaySize;
    // 여백(Padding) 값
    const float margin = 40.0f;

    // 1) 좌측 상단에 Distance: 표시
    {
        // 윈도우 플래그: 테두리·제목바 없고, 크기/위치 변경 불가
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove       |
            ImGuiWindowFlags_NoResize     |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoBackground;

        // 왼쪽 위(margin, margin) 위치 고정
        ImGui::SetNextWindowPos(ImVec2(margin, margin), ImGuiCond_Always);
        ImGui::Begin("##DistanceOverlay", nullptr, flags);

        float distanceValue = 0.0f;
        ImGui::Text("Distance: %.1f", distanceValue);

        ImGui::End();
    }

    // 2) 우측 상단에 일시정지 버튼
    {
        // 버튼 크기
        ImVec2 btnSize = ImVec2(128, 128);
        // 버튼 윈도우에 패딩/보더 없애기
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // 투명 창 띄우기
        ImGuiWindowFlags btnWindowFlags =
              ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoBackground;

        // 창 크기를 버튼 크기로 고정
        ImGui::SetNextWindowSize(btnSize, ImGuiCond_Always);

        // 화면 오른쪽 위 (margin, margin) 에 배치
        float posX = ScreenSize.x - btnSize.x - margin;
        float posY = margin;
        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);

        ImGui::Begin("##PauseButtonOverlay", nullptr, btnWindowFlags);
        // (이미 푸쉬한 WindowPadding/WindowBorderSize 뒤에)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));       // 버튼 기본 배경 투명
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0)); // 마우스 올렸을 때 배경 투명
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));  // 클릭 중일 때 배경 투명

        // 텍스처 가져오기
        auto pauseTexPtr = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Pause.png");
        ImTextureID pauseID = reinterpret_cast<ImTextureID>(pauseTexPtr->TextureSRV);

        // ImageButton을 헬퍼로 그리기
        // 클릭 시 PauseRequestedPtr = true
        DrawImageButton(ImGui::GetWindowDrawList(), pauseID, btnSize, posX, posY, "##PauseBtnHUD",[]()
        {
            FEngineLoop::PauseRequestedPtr = true;
        });

        ImGui::PopStyleColor(3);
        
        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}
