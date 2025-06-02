#include "GameStart.h"

#include "Animation/AnimNotifyState.h"
#include "ImGui/imgui.h"

void FGameStart::Update(float deltaTime)
{
    if (!bIsActive)
        return;

    // 1. 깜빡임 타이머 업데이트 (0.5초마다 상태 토글)
    BlinkTimer += deltaTime;
    if (BlinkTimer >= 1.0f)
    {
        BlinkTimer = 0.0f;
        bShowPrompt = !bShowPrompt;
    }
    
    // 2. Win32 API: GetAsyncKeyState를 이용해 “아무 키나 눌렸는지” 검사
    //    0부터 255까지의 가상 키(Virtual-Key) 코드를 순회하며,
    //    해당 키가 눌려 있는 상태(VK 상태의 최상위 비트가 1)를 감지
    if (FSlateAppMessageHandler::IsAnyKeyPressed())
    {
        bIsActive = false;
    }
}

void FGameStart::Render()
{
    if (!bIsActive)
        return;

    ImGuiIO& io = ImGui::GetIO();
    // 1. 풀스크린 투명 윈도우: 화면 전체를 덮어서 다른 UI가 보이지 않게 함
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration    // 제목바·테두리 제거
        | ImGuiWindowFlags_NoMove        // 드래그 불가
        | ImGuiWindowFlags_NoResize;     // 크기 조절 불가

    ImGui::Begin("##StartScreenFull", nullptr, flags);

    // 2) 배경 전체를 검은색(불투명)으로 채우기
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2     topLeft(0, 0);
    ImVec2     bottomRight(io.DisplaySize.x, io.DisplaySize.y);
    // IM_COL32(R,G,B,A) → A=255로 하면 완전 불투명
    drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(0, 0, 0, 255));

    // 2. 화면 중앙에 이미지로 게임 타이틀 출력
    if (TitleTexture && TitleTexture->TextureSRV && TitleTexture->Width > 0 && TitleTexture->Height > 0)
    {
        // 현재 윈도우(풀스크린)의 가로·세로 픽셀 크기
        float screenW = io.DisplaySize.x;
        float screenH = io.DisplaySize.y;

        // TitleTexture의 원본 픽셀 크기
        float texW = static_cast<float>(TitleTexture->Width);
        float texH = static_cast<float>(TitleTexture->Height);

        // 화면 중앙(가로: 50%, 세로: 40%) 위치 계산
        float centerX = screenW * 0.5f;
        float centerY = screenH * 0.4f;

        // 이미지의 절반 크기만큼 오프셋하여, 화면 중앙에 배치
        float posX = centerX - (texW * 0.5f);
        float posY = centerY - (texH * 0.5f);

        // ImGui::GetWindowDrawList()를 사용해 절대 좌표(ImGui 윈도우 내부 좌표계 기준)에 이미지 추가
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2   uv0(0.0f, 0.0f);
        ImVec2   uv1(1.0f, 1.0f);
        ImVec2   imgMin(posX, posY);
        ImVec2   imgMax(posX + texW, posY + texH);

        // DirectX11 SRV를 ImTextureID로 캐스팅
        drawList->AddImage(reinterpret_cast<ImTextureID>(TitleTexture->TextureSRV), imgMin, imgMax, uv0, uv1);
    }
    else
    {
        // 만약 이미지가 없으면, 텍스트 대체 예시 (in case)
        const char* fallback = "ALTF 4";
        ImVec2 textSize = ImGui::CalcTextSize(fallback);
        ImVec2 centerPos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.4f);
        ImGui::SetCursorPos(ImVec2(centerPos.x - textSize.x * 0.5f, centerPos.y - textSize.y * 0.5f));
        ImGui::TextUnformatted(fallback);
    }

    // 3. 깜빡이는 "Press Any Key To Start" 문구
    if (bShowPrompt)
    {
        const char* prompt = "Press Any Key To Start";

        // 현재 윈도우(풀스크린)의 가로·세로 픽셀 크기
        float screenW = io.DisplaySize.x;
        float screenH = io.DisplaySize.y;

        // 화면 중앙(가로: 50%, 세로: 60%) 위치 계산
        float centerX = screenW * 0.5f;
        float centerY = screenH * 0.6f;
        
        ImFont* InGameFont = GEngineLoop.GetGameUIManager()->GetInGameFont64();
        if (InGameFont && InGameFont->IsLoaded())
        {
            // 1) InGameFont를 활성화
            ImGui::PushFont(InGameFont);
            // 2) 이제 CalcTextSize가 InGameFont 기준으로 계산해 준다
            ImVec2 promptSize = ImGui::CalcTextSize(prompt);
            // 3) promptSize.x, promptSize.y는 InGameFont 크기로 올바르게 계산됨

            // 예를 들어, (가로 중앙에서 텍스트 너비 반만큼 빼기)
            ImVec2 promptPos(centerX - promptSize.x * 0.5f, centerY - promptSize.y * 0.5f);
            ImGui::SetCursorPos(promptPos);

            // 4) 글자 출력
            ImGui::TextUnformatted(prompt);
            // 5) 원래 폰트로 되돌리기
            ImGui::PopFont();
        }
        else
        {
            // InGameFont가 없으면 기본 폰트 기준으로 계산
            ImVec2 promptSize = ImGui::CalcTextSize(prompt);
            ImVec2 promptPos(centerX - promptSize.x * 0.5f, centerY - promptSize.y * 0.5f);
            ImGui::SetCursorPos(promptPos);
            ImGui::TextUnformatted(prompt);
        }
    }

    ImGui::End();
}
