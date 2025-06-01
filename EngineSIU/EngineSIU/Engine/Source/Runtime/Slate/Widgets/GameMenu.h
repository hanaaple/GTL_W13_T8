#pragma once
#include "UIElement.h"

class FGameMenu : public FUIElement
{
public:
    bool* isOpenPtr = nullptr; // 외부에서 메뉴 토글 상태 제어

    virtual void Update(float deltaTime) override;

    virtual void Render() override;
};
