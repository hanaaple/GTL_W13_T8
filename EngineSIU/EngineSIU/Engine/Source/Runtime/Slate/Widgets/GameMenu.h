#pragma once
#include "UIElement.h"

class FGameMenu : public FUIElement
{
public:
    virtual void Update(float deltaTime) override;

    virtual void Render() override;
    virtual ~FGameMenu() override;
};
