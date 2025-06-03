#include "GameMenu.h"

#include "EngineLoop.h"
#include "GameEnd.h"
#include "DirectXTK/PostProcess.h"
#include "Engine/Engine.h"
#include "FFaxk/FFaxkGameMode.h"
#include "ImGui/imgui.h"
#include "UObject/Object.h"
#include "World/World.h"


void FGameMenu::Update(float deltaTime)
{
}

void FGameMenu::Render()
{
    AFFaxkGameMode* GM = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>();

    if (!IsValid(GM) || GM->GetGameState() != EGameState::Paused)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 ScreenSize = io.DisplaySize;
    
    // 1) 풀스크린 반투명 회색 배경
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar   |   // 창의 타이틀바 제거
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove       |
        ImGuiWindowFlags_NoResize     |
        ImGuiWindowFlags_NoScrollbar  |   // 스크롤 바 제거
        ImGuiWindowFlags_NoBackground;
    
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ScreenSize, ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##PauseOverlay", nullptr, flags);

    ImDrawList* DrawList = ImGui::GetWindowDrawList();
    // 회색(50,50,50)에 알파 192 정도로 반투명 처리
    DrawList->AddRectFilled(ImVec2(0, 0), ImVec2(ScreenSize.x, ScreenSize.y), IM_COL32(50, 50, 50, 192));

    ImGui::PopStyleVar(); // WindowPadding
    
    // • 패널 텍스처 SRV를 ImTextureID로 변환
    std::shared_ptr<FTexture> PanelTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Panel.png");
    ImTextureID PanelTexID = reinterpret_cast<ImTextureID>(PanelTex->TextureSRV);
    ImVec2 PanelSize(static_cast<float>(PanelTex->Width) / 1.5f, static_cast<float>(PanelTex->Height));

    float panelX = (ScreenSize.x - PanelSize.x) * 0.5f;
    float panelY = (ScreenSize.y - PanelSize.y) * 0.5f;
    DrawImage(DrawList, PanelTexID, panelX, panelY, PanelSize,ImVec2(0, 0),ImVec2(1, 1),IM_COL32(255, 255, 255, 255));
    
    float spacing = 80.0f;             // 버튼 사이 간격 (예: 20px)

    // (미리 로드된 리소스 관리자를 통해) 각 버튼용 텍스처 SRV를 ImTextureID로 변환
    std::shared_ptr<FTexture> ResumeTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Resume_1.png");
    std::shared_ptr<FTexture> RestartTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Restart_1.png");
    std::shared_ptr<FTexture> ToMainTex = FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/MainMenu_1.png");
    std::shared_ptr<FTexture> QuitTex =  FEngineLoop::ResourceManager.GetTexture(L"Assets/Texture/Quit_1.png");
    
    // // ImTextureID로 변환
    ImTextureID ResumeTexID  = reinterpret_cast<ImTextureID>(ResumeTex->TextureSRV);
    ImTextureID RestartTexID = reinterpret_cast<ImTextureID>(RestartTex->TextureSRV);
    ImTextureID ToMainTexID  = reinterpret_cast<ImTextureID>(ToMainTex->TextureSRV);
    ImTextureID QuitTexID    = reinterpret_cast<ImTextureID>(QuitTex->TextureSRV);
    
    // 각 버튼 크기 (원본 이미지 크기)
    ImVec2 ResumeSize  = ImVec2(ResumeTex->Width / 4.0f, ResumeTex->Height / 4.0f);
    ImVec2 RestartSize = ImVec2(RestartTex->Width / 4.0f, RestartTex->Height / 4.0f);
    ImVec2 ToMainSize  = ImVec2(ToMainTex->Width / 4.0f, ToMainTex->Height / 4.0f);
    ImVec2 QuitSize    = ImVec2(QuitTex->Width / 4.0f, QuitTex->Height / 4.0f);

    float totalHeight = ResumeSize.y + RestartSize.y + ToMainSize.y + QuitSize.y + spacing * 3.0f; 
    // 이를 통해 “버튼 열 전체 높이”를 구할 수 있습니다
    
    float startY = (io.DisplaySize.y - totalHeight) * 0.5f;
    
    float posX = (io.DisplaySize.x - ResumeSize.x) * 0.5f;
    float posY = startY;
    DrawImageButton(DrawList, ResumeTexID, ResumeSize, posX, posY, "##ResumeBtn", [&]()
    {
        // TODO : ResumeButton 클릭 로직
        GM->SetGameState(EGameState::Playing);
    });

    posX = (io.DisplaySize.x - RestartSize.x) * 0.5f;
    posY = startY + ResumeSize.y + spacing;
    DrawImageButton(DrawList, RestartTexID, RestartSize, posX, posY, "##RestartBtn", [&]()
    {
        // TODO : RestartButton 클릭 로직
        GM->SetGameState(EGameState::Playing);

        APlayerController* PC = GM->GetPlayerController();
        PC->GetPawn()->Destroy();
        GM->RequestPlayerRespawn(PC);
    });

    posX = (io.DisplaySize.x - ToMainSize.x) * 0.5f;
    posY = startY + ResumeSize.y + RestartSize.y + spacing * 2.0f;
    DrawImageButton(DrawList, ToMainTexID, ToMainSize, posX, posY, "##ToMainBtn", [&]()
    {
        // TODO : ToMainButton 클릭 롲직
    });

    posX = (io.DisplaySize.x - QuitSize.x) * 0.5f;
    posY = startY + ResumeSize.y + RestartSize.y + ToMainSize.y + spacing * 3.0f;
    DrawImageButton(DrawList, QuitTexID, QuitSize, posX, posY, "##QuitBtn", [&]()
    {
        // TODO : QuitButton 클릭 로직
        PostQuitMessage(0);
    });
    
    ImGui::End();
}

FGameMenu::~FGameMenu()
{
}
