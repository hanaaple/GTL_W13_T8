#include "GameUIManager.h"

#include "ImGui/imgui.h"

FGameUIManager::FGameUIManager()
{
}

void FGameUIManager::Initialize()
{
    const ImGuiIO& IO = ImGui::GetIO();
    //GameFont48 = IO.Fonts->AddFontFromFileTTF("Assets/Font/ChosunCentennial_ttf.ttf", 48.0f, nullptr, IO.Fonts->GetGlyphRangesKorean());
    GameFont64 = IO.Fonts->AddFontFromFileTTF("Assets/Font/ChosunCentennial_ttf.ttf", 64.0f, nullptr, IO.Fonts->GetGlyphRangesKorean());
}
