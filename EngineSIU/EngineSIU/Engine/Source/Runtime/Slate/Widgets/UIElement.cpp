#include "UIElement.h"

#include "Engine/Texture.h"

void FUIElement::DrawFullScreenColor(ImDrawList* dl, const ImVec2& screenSize, ImU32 col)
{
    dl->AddRectFilled(ImVec2(0, 0), screenSize, col);
}

void FUIElement::DrawCenteredText(ImDrawList* dl, ImFont* font, float fontSize, const ImVec2& screen, const char* text, ImU32 col, float vertPct)
{
    // 텍스트 크기 계산
    ImVec2 txtSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, text);
    // 위치 계산
    ImVec2 pos;
    pos.x = (screen.x - txtSize.x) * 0.5f;
    pos.y = (screen.y - txtSize.y) * vertPct - txtSize.y * 0.5f;
    // 그리기
    dl->AddText(font, fontSize, pos, col, text);
}

void FUIElement::DrawImage(ImDrawList* dl, ImTextureID texSRV, float posX, float posY, const ImVec2& texSize, const ImVec2& uv0, const ImVec2& uv1, ImU32 tintColor)
{
    // posX, posY는 화면 절대 좌표
    ImVec2 imgMin(posX, posY);
    ImVec2 imgMax(posX + texSize.x, posY + texSize.y);

    // 기본 uv0=(0,0), uv1=(1,1) 이라면 전체 텍스처가 그려짐
    dl->AddImage(texSRV, imgMin, imgMax, uv0, uv1, tintColor);
}

void FUIElement::DrawImageButton(ImDrawList* dl, ImTextureID texSRV, const ImVec2& texSize, float posX, float posY, const char* id, const TFunction<void()>& onClick)
{
    // 1) 지정된 위치로 커서 이동
    ImGui::SetCursorScreenPos(ImVec2(posX, posY));

    // 2) ImageButton 그리기 (투명 배경, 패딩 0)
    ImGui::ImageButton(id, texSRV, texSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));

    // 3) 클릭 감지
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        onClick();
    }

    // 4) Hover 상태라면 반투명 흰색 오버레이
    if (ImGui::IsItemHovered())
    {
        ImVec2 a = ImGui::GetItemRectMin();
        ImVec2 b = ImGui::GetItemRectMax();
        dl->AddImage(texSRV, a, b, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 100));
    }
}
