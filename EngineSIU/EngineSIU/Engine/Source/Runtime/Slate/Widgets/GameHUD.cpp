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
    ImGuiIO& io = ImGui::GetIO();
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
        // 1) 윈도우 플래그: 데코레이션·이동·크기조절·배경 모두 제거
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration
          | ImGuiWindowFlags_NoMove
          | ImGuiWindowFlags_NoResize
          | ImGuiWindowFlags_NoBackground;

        // 2) 버튼 크기
        ImVec2 btnSize = ImVec2(128, 128);
        float  margin  = 40.0f;  // 화면 모서리에서 떨어진 거리

        // 3) 패딩을 0으로 만든다 (왼쪽 0, 오른쪽 0)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // 4) SetNextWindowSize: 윈도우 크기를 버튼 크기로 고정
        ImGui::SetNextWindowSize(btnSize, ImGuiCond_Always);

        // 5) 화면 오른쪽 끝에 붙도록 pos 계산
        //    : 화면 너비(DisplaySize.x)에서 윈도우 폭(btnSize.x)과 margin을 뺀 값
        float posX = io.DisplaySize.x - btnSize.x - margin;
        float posY = margin;
        ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_Always);

        // 6) Begin → 버튼 → End
        ImGui::Begin("##PauseButtonOverlay", nullptr, flags);
        ImTextureID PauseIconTexture = reinterpret_cast<ImTextureID>(FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Pause.png")->TextureSRV);

        // (이미 푸쉬한 WindowPadding/WindowBorderSize 뒤에)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));       // 버튼 기본 배경 투명
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0)); // 마우스 올렸을 때 배경 투명
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));  // 클릭 중일 때 배경 투명
        
        ImGui::ImageButton("##PauseButton", PauseIconTexture, btnSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
        
        // ImageButton을 먼저 그리고 나서 hover 상태를 체크한 뒤,
        // 실제로 hover 중이면 이미지를 다시 덮어씌워서 밝게 표현하는 방식
        if (ImGui::IsItemHovered())
        {
            // 클릭 처리
            if (ImGui::IsMouseClicked(0))
            {
                FEngineLoop::PauseRequestedPtr = true;
            }

            // 현재 ImageButton 사각형 영역 가져오기
            ImVec2 a = ImGui::GetItemRectMin();
            ImVec2 b = ImGui::GetItemRectMax();

            // 같은 크기, 같은 UV를 이용해 밝은 Tint로 이미지를 한 번 더 그린다.
            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddImage(PauseIconTexture,a,b,ImVec2(0, 0),ImVec2(1, 1), IM_COL32(255, 255, 255, 100));
        }
        
        // 푸시했던 스타일 컬러 해제
        ImGui::PopStyleColor(3);
        
        ImGui::End();

        // 7) PushStyleVar 해제
        ImGui::PopStyleVar(2);
    }
}
