#include "GameUIManager.h"

#include "ImGui/imgui.h"

FGameUIManager::FGameUIManager()
{
}

void FGameUIManager::Initialize()
{
    const ImGuiIO& IO = ImGui::GetIO();
    GameFont = IO.Fonts->AddFontFromFileTTF("Assets/Font/ChosunCentennial_ttf.ttf", 48.0f, nullptr, IO.Fonts->GetGlyphRangesKorean());
}
