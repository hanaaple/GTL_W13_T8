#include "GameStart.h"

#include "Animation/AnimNotifyState.h"
#include "ImGui/imgui.h"

void FGameStart::SetTitleImage(const std::shared_ptr<FTexture>& InTitleTexture, float InWidth, float InHeight)
{
    TitleTexture  = InTitleTexture;
    TitleSize = ImVec2(InWidth, InHeight);
}

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

    const ImGuiIO& IO        = ImGui::GetIO();
    const ImVec2   ScreenSize    = IO.DisplaySize;
    // 1. 풀스크린 투명 윈도우: 화면 전체를 덮어서 다른 UI가 보이지 않게 함
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(IO.DisplaySize, ImGuiCond_Always);
    
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration    // 제목바·테두리 제거
        | ImGuiWindowFlags_NoMove        // 드래그 불가
        | ImGuiWindowFlags_NoResize;     // 크기 조절 불가

    ImGui::Begin("##StartScreenFull", nullptr, flags);
    ImDrawList* DrawList = ImGui::GetWindowDrawList();
    
    // 2) 배경 전체를 검은색(불투명)으로 채우기
    //DrawFullScreenColor(DrawList, ScreenSize, IM_COL32(0, 0, 0, 255));
    if (std::shared_ptr<FTexture> TitleImage = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/MainImage.png"))
    {
        // ----- 1) 텍스처 크기 가져오기 -----
        float texW = TitleImage->Width;
        float texH = TitleImage->Height;

        // ----- 2) 화면 크기 가져오기 -----
        float screenW = ScreenSize.x;
        float screenH = ScreenSize.y;

        // ----- 3) 스케일 계산 (화면에 들어가도록 가장 작은 스케일 선택) -----
        float scaleX = screenW / texW;
        float scaleY = screenH / texH;
        float scale  = (scaleX < scaleY) ? scaleX : scaleY;

        // ----- 4) 실제 렌더할 이미지 크기 -----
        float renderW = texW * scale;
        float renderH = texH * scale;

        // ----- 5) 화면 중앙에 배치하기 위해 좌표 계산 -----
        float posX = (screenW - renderW) * 0.5f;
        float posY = (screenH - renderH) * 0.5f;

        // ----- 6) UV 좌표: (0,0) ~ (1,1) (원본 전체 사용) -----
        ImVec2 uv0(0.0f, 0.0f);
        ImVec2 uv1(1.0f, 1.0f);

        // ----- 7) 이미지 그리기 -----
        DrawList->AddImage(reinterpret_cast<ImTextureID>(TitleImage->TextureSRV), ImVec2(posX, posY), ImVec2(posX + renderW, posY + renderH), uv0, uv1);
    }
        
    // 2. 화면 중앙에 이미지로 게임 타이틀 출력
    if (TitleTexture && TitleTexture->TextureSRV && TitleTexture->Width > 0 && TitleTexture->Height > 0)
    {
        // 현재 윈도우(풀스크린)의 가로·세로 픽셀 크기
        const float screenW = IO.DisplaySize.x;
        const float screenH = IO.DisplaySize.y;

        // TitleTexture의 원본 픽셀 크기
        const float texW = static_cast<float>(TitleTexture->Width);
        const float texH = static_cast<float>(TitleTexture->Height);

        // 화면 중앙(가로: 50%, 세로: 40%) 위치 계산
        const float centerX = screenW * 0.5f;
        const float centerY = screenH * 0.4f;

        // 이미지의 절반 크기만큼 오프셋하여, 화면 중앙에 배치
        const float posX = centerX - (texW * 0.5f);
        const float posY = centerY - (texH * 0.5f);

        // ImGui::GetWindowDrawList()를 사용해 절대 좌표(ImGui 윈도우 내부 좌표계 기준)에 이미지 추가
        const ImVec2   uv0(0.0f, 0.0f);
        const ImVec2   uv1(1.0f, 1.0f);
        const ImVec2   imgMin(posX, posY);
        const ImVec2   imgMax(posX + texW, posY + texH);

        // DirectX11 SRV를 ImTextureID로 캐스팅
        DrawList->AddImage(reinterpret_cast<ImTextureID>(TitleTexture->TextureSRV), imgMin, imgMax, uv0, uv1);
    }
    else
    {
        // 타이틀 이미지가 없으면 텍스트 대체
        if (ImFont* InGameFont = GEngineLoop.GetGameUIManager()->GetInGameFont64())
        {
            DrawCenteredText(DrawList, InGameFont, InGameFont->FontSize, ScreenSize, "FFaxk", IM_COL32(255, 255, 255, 255), 0.4f);
        }
    }

    // 3. 깜빡이는 "Press Any Key To Start" 문구
    if (bShowPrompt)
    {
        if (ImFont* InGameFont = GEngineLoop.GetGameUIManager()->GetInGameFont64())
        {
            DrawCenteredText(DrawList, InGameFont, InGameFont->FontSize, ScreenSize, "Press Any Key To Start", IM_COL32(255, 255, 255, 255), 0.6f);
        }
    }

    ImGui::End();
}
