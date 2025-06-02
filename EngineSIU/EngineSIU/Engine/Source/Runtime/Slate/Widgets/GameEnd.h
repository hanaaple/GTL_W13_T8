#pragma once
#include "UIElement.h"

class FGameEnd : public FUIElement
{
public:
    FGameEnd();
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual ~FGameEnd() override;
};
