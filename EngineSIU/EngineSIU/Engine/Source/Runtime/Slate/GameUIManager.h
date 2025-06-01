#pragma once
#include "Container/Array.h"
#include "Widgets/UIElement.h"

class FGameUIManager
{
public:
    FGameUIManager() = default;
    ~FGameUIManager() = default;
    
    void AddElement(const std::shared_ptr<FUIElement>& element)
    {
        UIElements.Add(element);
    }

    void RemoveElement(const std::shared_ptr<FUIElement>& element)
    {
        UIElements.Remove(element);
    }
    
    void RemoveAll() { UIElements.Empty(); }

    void UpdateAll(float DeltaTime)
    {
        for (const std::shared_ptr<FUIElement>& UIElement : UIElements)
        {
            UIElement->Update(DeltaTime);
        }
    }

    // 매 프레임 ImGui Render 단계에서 호출
    void RenderAll()
    {
        for (const std::shared_ptr<FUIElement>& UIElement : UIElements)
        {
            UIElement->Render();
        }
    }

private:
    TArray<std::shared_ptr<FUIElement>> UIElements;
};
