#include "GameMenu.h"

#include "EngineLoop.h"
#include "GameEnd.h"
#include "DirectXTK/PostProcess.h"
#include "ImGui/imgui.h"
#include "UObject/Object.h"

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
        ImGuiWindowFlags_NoTitleBar   |   // 창의 타이틀바 제거
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove       |
        ImGuiWindowFlags_NoResize     |
        ImGuiWindowFlags_NoScrollbar  |   // 스크롤 바 제거
        ImGuiWindowFlags_NoBackground;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(screen, ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##PauseOverlay", nullptr, flags);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    // 회색(50,50,50)에 알파 192 정도로 반투명 처리
    drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screen.x, screen.y), IM_COL32(50, 50, 50, 192));

    ImGui::PopStyleVar(); // WindowPadding
    
    //    • 패널 텍스처 SRV를 ImTextureID로 변환
    std::shared_ptr<FTexture> PanelTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Panel.png");
    ImTextureID PanelTexID = reinterpret_cast<ImTextureID>(PanelTex->TextureSRV);

    //    • 패널 원본 크기 (1280×512)라고 가정
    float panelW = PanelTex->Width / 1.5f;
    float panelH = PanelTex->Height;
    //    • 화면 중앙에 패널이 오도록 위치 계산
    float panelX = (screen.x - panelW) * 0.5f;
    float panelY = (screen.y - panelH) * 0.5f;
    //    • DrawList를 이용해 배경 패널 그리기 (투명 배경 유지)
    drawList->AddImage(PanelTexID, ImVec2(panelX, panelY),ImVec2(panelX + panelW, panelY + panelH), ImVec2(0, 0),ImVec2(1, 1),IM_COL32(255, 255, 255, 255));
    
    float  spacing = 80.0f;               // 버튼 사이 간격 (예: 20px)

    // (미리 로드된 리소스 관리자를 통해) 각 버튼용 텍스처 SRV를 ImTextureID로 변환
    std::shared_ptr<FTexture> ResumeTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Resume_1.png");
    std::shared_ptr<FTexture> RestartTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Restart_1.png");
    std::shared_ptr<FTexture> ToMainTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/MainMenu_1.png");
    std::shared_ptr<FTexture> QuitTex =  FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Quit_1.png");

    // ImTextureID로 변환
    ImTextureID ResumeTexID  = reinterpret_cast<ImTextureID>(ResumeTex->TextureSRV);
    ImTextureID RestartTexID = reinterpret_cast<ImTextureID>(RestartTex->TextureSRV);
    ImTextureID ToMainTexID  = reinterpret_cast<ImTextureID>(ToMainTex->TextureSRV);
    ImTextureID QuitTexID    = reinterpret_cast<ImTextureID>(QuitTex->TextureSRV);

    // 각 버튼 크기 (원본 이미지 크기)
    ImVec2 ResumeSize  = ImVec2(ResumeTex->Width / 4.0f, ResumeTex->Height / 4.0f);
    ImVec2 RestartSize = ImVec2(RestartTex->Width / 4.0f, RestartTex->Height / 4.0f);
    ImVec2 ToMainSize  = ImVec2(ToMainTex->Width / 4.0f, ToMainTex->Height / 4.0f);
    ImVec2 QuitSize    = ImVec2(QuitTex->Width / 4.0f, QuitTex->Height / 4.0f);

    
    // 4) 수직 방향을 기준으로 첫 번째 버튼이 그려질 Y 좌표 결정
    // ---------------------------
    float totalHeight = ResumeSize.y + RestartSize.y + ToMainSize.y + QuitSize.y + spacing * 3.0f; 
    // 이를 통해 “버튼 열 전체 높이”를 구할 수 있습니다

    float startY = (io.DisplaySize.y - totalHeight) * 0.5f;
    // 화면 세로 중앙에서 totalHeight/2 만큼 위로 올리면 첫 버튼이 위쪽으로 배치됨

    // 버튼 스타일: 배경 투명, 테두리 없음
    ImGui::PushStyleColor(ImGuiCol_Button,         ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   ImVec4(0,0,0,0));

    // ① 화면 가로 중앙에 맞추기 위해 posX 계산
    float posX = (io.DisplaySize.x - ResumeSize.x) * 0.5f;
    float posY = startY;
    // --- Resume 버튼 ---
    ImGui::SetCursorScreenPos(ImVec2(posX, posY));
    ImGui::ImageButton("##ResumeBtn", ResumeTexID, ResumeSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));

    // ▶ ImageButton을 그린 직후에는 IsItemHovered()가 해당 버튼을 가리킴
    if (ImGui::IsItemHovered())
    {
        // (선택) 클릭 처리도 여기서 처리 가능
        if (ImGui::IsMouseClicked(0))
        {
            FEngineLoop::PauseRequestedPtr = false;
        }

        // 현재 버튼의 절대 좌표를 가져와서
        ImVec2 a = ImGui::GetItemRectMin();
        ImVec2 b = ImGui::GetItemRectMax();

        // 같은 텍스처를 반투명 흰색 오버레이로 덮어씌움
        drawList->AddImage(ResumeTexID, a, b, ImVec2(0,0), ImVec2(1,1), IM_COL32(255,255,255,100));
    }

    // --- Restart 버튼 ---
    posX = (io.DisplaySize.x - RestartSize.x) * 0.5f;
    posY = startY + ResumeSize.y + spacing;
    ImGui::SetCursorScreenPos(ImVec2(posX, posY));
    ImGui::ImageButton("##RestartBtn", RestartTexID, RestartSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            // TODO : 클릭 처리
            FEngineLoop::PauseRequestedPtr = false;
            FEngineLoop::bIsDied = true;
        }

        ImVec2 a = ImGui::GetItemRectMin();
        ImVec2 b = ImGui::GetItemRectMax();
        drawList->AddImage(RestartTexID,a,b,ImVec2(0,0),ImVec2(1,1),IM_COL32(255,255,255,100));
    }

    // --- MainMenu 버튼 ---
    posX = (io.DisplaySize.x - ToMainSize.x) * 0.5f;
    posY = startY + ResumeSize.y + RestartSize.y + spacing * 2.0f;
    ImGui::SetCursorScreenPos(ImVec2(posX, posY));
    ImGui::ImageButton("##ToMainBtn", ToMainTexID, ToMainSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
    // ▶ ImageButton을 그린 직후에는 IsItemHovered()가 해당 버튼을 가리킴
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            // TODO : 클릭 처리
        }

        // 현재 버튼의 절대 좌표를 가져와서
        ImVec2 a = ImGui::GetItemRectMin();
        ImVec2 b = ImGui::GetItemRectMax();

        // 같은 텍스처를 반투명 흰색 오버레이로 덮어씌움
        drawList->AddImage(ToMainTexID, a, b, ImVec2(0,0), ImVec2(1,1), IM_COL32(255,255,255,100));
    }
    
    // --- Quit 버튼 ---
    posX = (io.DisplaySize.x - QuitSize.x) * 0.5f;
    posY = startY + ResumeSize.y + RestartSize.y + ToMainSize.y + spacing * 3.0f;
    ImGui::SetCursorScreenPos(ImVec2(posX, posY));
    ImGui::ImageButton("##QuitBtn", QuitTexID, QuitSize,ImVec2(0, 0),ImVec2(1, 1),ImVec4(0, 0, 0, 0),ImVec4(1, 1, 1, 1));
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            PostQuitMessage(0);
        }

        ImVec2 a = ImGui::GetItemRectMin();
        ImVec2 b = ImGui::GetItemRectMax();
        drawList->AddImage(QuitTexID, a, b, ImVec2(0,0), ImVec2(1,1), IM_COL32(255,255,255,100));
    }

    // 푸시했던 스타일 컬러 Pop
    ImGui::PopStyleColor(3);
    ImGui::End();
}

FGameMenu::~FGameMenu()
{
}
