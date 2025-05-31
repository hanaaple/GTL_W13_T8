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

    TFunction<void(USceneComponent*)> CreateNode = [&CreateNode, &Engine](
        USceneComponent* InComp
    )-> void
    {
        FString Name = InComp->GetName();

        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_DefaultOpen;
        if (InComp->GetAttachChildren().Num() == 0)
        {
            Flags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (Engine->GetSelectedComponent() == InComp)
        {
            Flags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
        bool NodeOpen = ImGui::TreeNodeEx(*Name, Flags);

        if (ImGui::IsItemClicked())
        {
            Engine->SelectActor(InComp->GetOwner());
            Engine->SelectComponent(InComp);
        }

        if (NodeOpen)
        {
            for (USceneComponent* Child : InComp->GetAttachChildren())
            {
                CreateNode(Child);
            }
            ImGui::TreePop(); // 트리 닫기
        }
    };

    for (AActor* Actor : Engine->ActiveWorld->GetActiveLevel()->Actors)
    {
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_SpanAvailWidth // 클릭 영역 확대
            | ImGuiTreeNodeFlags_FramePadding   // 패딩
            | ImGuiTreeNodeFlags_Leaf;          // 트리 열기 버튼 숨기기

        if (Engine->GetSelectedActor() == Actor)
        {
            Flags |= ImGuiTreeNodeFlags_Selected;
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.5f, 0.2f));
        {
            ImGui::SetNextItemOpen(true, ImGuiCond_Always);
            const bool NodeOpen = ImGui::TreeNodeEx(*Actor->GetActorLabel(), Flags);

            if (ImGui::IsItemClicked())
            {
                Engine->SelectActor(Actor);
                Engine->DeselectComponent(Engine->GetSelectedComponent());
            }

            if (NodeOpen)
            {
                if (USceneComponent* SceneComp = Actor->GetRootComponent())
                {
                    CreateNode(SceneComp);
                }
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
