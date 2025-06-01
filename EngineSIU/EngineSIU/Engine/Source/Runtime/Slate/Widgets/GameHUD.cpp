#include "GameHUD.h"

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
    ImGuiIO& io = ImGui::GetIO();
    const float margin = 10.0f;

    // 1) 좌측 상단에 Distance: 표시
    {
        // 윈도우 플래그: 테두리·제목바 없고, 크기/위치 변경 불가
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove       |
            ImGuiWindowFlags_NoResize     |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoBackground;

        // 왼쪽 위(10,10) 위치 고정
        ImGui::SetNextWindowPos(ImVec2(margin, margin), ImGuiCond_Always);
        ImGui::Begin("##DistanceOverlay", nullptr, flags);

        float distanceValue = (DistancePtr ? *DistancePtr : 0.0f);
        ImGui::Text("Distance: %.1f", distanceValue);

        ImGui::End();
    }

    // 2) 우측 상단에 일시정지 버튼
    {
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove       |
            ImGuiWindowFlags_NoResize     |
            ImGuiWindowFlags_NoBackground;

        // 버튼 크기 미리 계산(예: 80×30)
        ImVec2 btnSize(80.0f, 30.0f);
        // 화면 우측 여백을 고려한 X 좌표 계산
        float posX = io.DisplaySize.x - btnSize.x - margin;
        float posY = margin;

        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);
        ImGui::Begin("##PauseButtonOverlay", nullptr, flags);

        // 일시정지 버튼
        if (ImGui::Button("Pause", btnSize)) {
            if (PauseRequestedPtr) {
                *PauseRequestedPtr = true;
            }
        }

        ImGui::End();
    }
}
