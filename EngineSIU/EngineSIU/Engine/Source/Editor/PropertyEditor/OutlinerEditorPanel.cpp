#include "OutlinerEditorPanel.h"
#include "World/World.h"
#include "GameFramework/Actor.h"
#include "Engine/EditorEngine.h"
#include <functional>

OutlinerEditorPanel::OutlinerEditorPanel()
{
    SetSupportedWorldTypes(EWorldTypeBitFlag::Editor | EWorldTypeBitFlag::PIE );
}

void OutlinerEditorPanel::Render()
{
    /* Pre Setup */
    ImGuiIO& io = ImGui::GetIO();
    
    float PanelWidth = (Width) * 0.2f - 5.0f;
    float PanelHeight = (Height) * 0.3f;

    float PanelPosX = (Width) * 0.8f + 4.0f;
    float PanelPosY = 5.0f;

    ImVec2 MinSize(140, 100);
    ImVec2 MaxSize(FLT_MAX, 500);
    
    /* Min, Max Size */
    ImGui::SetNextWindowSizeConstraints(MinSize, MaxSize);
    
    /* Panel Position */
    ImGui::SetNextWindowPos(ImVec2(PanelPosX, PanelPosY), ImGuiCond_Always);

    /* Panel Size */
    ImGui::SetNextWindowSize(ImVec2(PanelWidth, PanelHeight), ImGuiCond_Always);

    /* Panel Flags */
    ImGuiWindowFlags PanelFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    
    /* Render Start */
    ImGui::Begin("Outliner", nullptr, PanelFlags);

    
    ImGui::BeginChild("Objects");
    UEditorEngine* Engine = Cast<UEditorEngine>(GEngine);
    if (!Engine)
    {
        ImGui::EndChild();
        ImGui::End();

        return;
    }

    for (AActor* Actor : Engine->ActiveWorld->GetActiveLevel()->Actors)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_SpanAvailWidth    // 클릭 영역 확대
            | ImGuiTreeNodeFlags_FramePadding      // 패딩
            | ImGuiTreeNodeFlags_OpenOnDoubleClick // 두번 클릭 시 열기 (한번 클릭했을 때, 아이콘 움직이는거 방지)
            | ImGuiTreeNodeFlags_Leaf;             // 트리 열기 버튼 숨기기

        if (Engine->GetSelectedActor() == Actor)
        {
            Flags |= ImGuiTreeNodeFlags_Selected;
            Flags &= ~ImGuiTreeNodeFlags_Leaf;
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.5f, 0.2f));
        {
            ImGui::SetNextItemOpen(false, ImGuiCond_Always);
            const bool NodeOpen = ImGui::TreeNodeEx(*Actor->GetActorLabel(), Flags);

            if (ImGui::IsItemClicked())
            {
                Engine->SelectActor(Actor);
                Engine->DeselectComponent(Engine->GetSelectedComponent());
            }

            if (NodeOpen)
            {
                ImGui::TreePop();
            }
        }
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
    ImGui::End();
}
    
void OutlinerEditorPanel::OnResize(HWND hWnd)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    Width = clientRect.right - clientRect.left;
    Height = clientRect.bottom - clientRect.top;
}
