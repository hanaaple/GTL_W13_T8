#pragma once
#include <memory>

#include "UIElement.h"
#include "ImGui/imgui.h"

struct FTexture;

class FGameStart : public FUIElement
{
public:
    FGameStart()
        : bIsActive(true)
        , BlinkTimer(0.0f)
        , bShowPrompt(true)
    {}

    // Start Screen이 화면에 보이는지 여부
    bool IsActive() const { return bIsActive; }
    void SetActive(bool bOn) { bIsActive = bOn; }

    // '타이틀 이미지'와 크기를 외부에서 설정할 수 있도록 Setter 추가
    void SetTitleImage(const std::shared_ptr<FTexture>& InTitleTexture, float InWidth, float InHeight)
    {
        TitleTexture  = InTitleTexture;
        TitleSize = ImVec2(InWidth, InHeight);
    }

    // Update 단계에서 타이머 갱신 및 키 입력 감지
    virtual void Update(float deltaTime) override;

    // Render 단계에서 Start Screen 그리기
    virtual void Render() override;

private:
    bool  bIsActive;    // Start Screen 활성/비활성 플래그
    float BlinkTimer;   // 깜빡임 타이머 (초 단위)
    bool  bShowPrompt;  // “Press Any Key” 문구를 보일지 여부

    std::shared_ptr<FTexture> TitleTexture; 
    ImVec2      TitleSize;   // 게임 타이틀 이미지 크기 (픽셀 단위)
};
