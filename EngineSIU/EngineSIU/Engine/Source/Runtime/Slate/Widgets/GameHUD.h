#pragma once
#include "UIElement.h"

class FGameHUD : public FUIElement
{
public:
    FGameHUD();
    virtual ~FGameHUD() override;
    
    // 외부에서 현재 이동 거리 값을 전달받도록 함
    float* DistancePtr = nullptr;
    // 일시정지 버튼 클릭 시 상태를 외부에서 읽어갈 수 있도록 플래그
    bool*  PauseRequestedPtr = nullptr;

    virtual void Update(float deltaTime) override;
    
    virtual void Render() override;
};
