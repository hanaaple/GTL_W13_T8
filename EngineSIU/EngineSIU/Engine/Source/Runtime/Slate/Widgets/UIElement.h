#pragma once
#include "ImGui/imgui.h"
#include "Templates/Function.h"

struct FTexture;

class FUIElement
{
public:
    // 매 프레임 게임 로직 단계에서 호출 (위치 계산용 데이터 준비 등)
    virtual void Update(float deltaTime) = 0;

    // ImGui 렌더링 단계에서 호출
    virtual void Render() = 0;
protected:
    void DrawFullScreenColor(ImDrawList* dl, const ImVec2& screenSize, ImU32 col);
    void DrawCenteredText(ImDrawList* dl, ImFont* font, float fontSize, const ImVec2& screen, const char* text, ImU32 col, float vertPct = 0.5f);
    void DrawImage(ImDrawList* dl, ImTextureID texSRV, float posX, float posY,const ImVec2& texSize, const ImVec2& uv0, const ImVec2& uv1, ImU32 tintColor);
    void DrawImageButton(ImDrawList* dl, ImTextureID texSRV, const ImVec2& texSize, float posX, float posY, const char* id, const TFunction<void()>& onClick);
    
    virtual ~FUIElement() = default;
};
 
