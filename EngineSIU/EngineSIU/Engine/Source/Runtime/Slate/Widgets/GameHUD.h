#pragma once
#include "UIElement.h"

class FGameHUD : public FUIElement
{
public:
    FGameHUD();
    virtual ~FGameHUD() override;

    virtual void Update(float deltaTime) override;
    
    virtual void Render() override;
};
