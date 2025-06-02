#include "GameEnd.h"

#include "ImGui/imgui.h"
#include "UObject/Object.h"

FGameEnd::FGameEnd()
{
}

void FGameEnd::Update(float deltaTime)
{
    if (FSlateAppMessageHandler::IsAnyKeyPressed())
    {
        // TODO : Restart나 Retrun To Main 로직
        FEngineLoop::bIsDied = false;
    }
}

void FGameEnd::Render()
{
    if (FEngineLoop::bIsDied == false)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screenSize = io.DisplaySize;
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // 1) 검은 배경
    drawList->AddRectFilled(ImVec2(0, 0), ImVec2(screenSize.x, screenSize.y), IM_COL32(0, 0, 0, 255));

    ImFont* font = GEngineLoop.GetGameUIManager()->GetInGameFont64();
    if (font && font->IsLoaded())
    {
        float fontSize = 128;
        float fontScale = fontSize / font->FontSize;
        // 2) PushFont 으로 폰트 변경
        ImGui::PushFont(font);
        const char* msg = "You Die !";
        ImVec2 textSize = ImGui::CalcTextSize(msg);
        ImVec2 textPos((screenSize.x - textSize.x * fontScale) * 0.5f, (screenSize.y - textSize.y * fontScale) * 0.5f);
    
        // 5) AddText로 직접 그리기 (빨간색)
        drawList->AddText(font, fontSize, textPos, IM_COL32(255, 0, 0, 255), msg);

        // 3) "Press To Restart"를 그리기 (작게, 예: 64px)
        fontSize   = 64.0f;
        fontScale = fontSize / font->FontSize;
        // 작은 글자는 화면 중앙 하단 근처에 배치
        const char* sub = "Press Any Key To Restart";
        ImVec2 subSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, sub);
        ImVec2 subPos((screenSize.x - subSize.x) * 0.5f, textPos.y + textSize.y + 70.0f // "You Die !" 아래로 20px 아래에 배치
        );
        drawList->AddText(font, fontSize, subPos, IM_COL32(255, 255, 255, 255), sub);
    
        ImGui::PopFont();
    }


}

FGameEnd::~FGameEnd()
{
}
