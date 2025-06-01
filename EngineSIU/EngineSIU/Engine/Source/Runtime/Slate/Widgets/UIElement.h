#pragma once

class FUIElement
{
public:
    // 매 프레임 게임 로직 단계에서 호출 (위치 계산용 데이터 준비 등)
    virtual void Update(float deltaTime) = 0;

    // ImGui 렌더링 단계에서 호출
    virtual void Render() = 0;

    virtual ~FUIElement() = default;
};
 
