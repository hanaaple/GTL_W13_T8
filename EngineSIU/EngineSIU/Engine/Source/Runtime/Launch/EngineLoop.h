#pragma once
#include "GameUIManager.h"
#include "Core/HAL/PlatformType.h"
#include "Engine/ResourceMgr.h"
#include "LevelEditor/SlateAppMessageHandler.h"
#include "LuaScripts/LuaScriptManager.h"
#include "Renderer/Renderer.h"
#include "UnrealEd/PrimitiveDrawBatch.h"
#include "Stats/ProfilerStatsManager.h"
#include "Stats/GPUTimingManager.h"


class FSlateAppMessageHandler;
class UnrealEd;
class UImGuiManager;
class UWorld;
class FEditorViewportClient;
class SSplitterV;
class SSplitterH;
class FGraphicDevice;
class SLevelEditor;
class FDXDBufferManager;

class FEngineLoop
{
public:
    FEngineLoop();

    int32 PreInit();
    int32 Init(HINSTANCE hInstance);
    void Render() const;
    void Tick();
    void Exit();

    void GetClientSize(uint32& OutWidth, uint32& OutHeight) const;

private:
    void WindowInit(HINSTANCE hInstance);
    static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    void UpdateUI();

public:
    static FGraphicsDevice GraphicDevice;
    static FRenderer Renderer;
    static FLuaScriptManager ScriptSys;
    static UPrimitiveDrawBatch PrimitiveDrawBatch;
    static FResourceManager ResourceManager;
    static uint32 TotalAllocationBytes;
    static uint32 TotalAllocationCount;

    HWND AppWnd;

    FGPUTimingManager GPUTimingManager;
    FEngineProfiler EngineProfiler;
    

private:
    UImGuiManager* UIManager;
    //TODO: GWorld 제거, Editor들 EditorEngine으로 넣기
    FGameUIManager* GameUIManager;

    std::unique_ptr<FSlateAppMessageHandler> AppMessageHandler;
    SLevelEditor* LevelEditor;
    UnrealEd* UnrealEditor;
    FDXDBufferManager* BufferManager; //TODO: UEngine으로 옮겨야함.

    bool bIsExit = false;
    // @todo Option으로 선택 가능하도록
    int32 TargetFPS = 60;

public:
    FGameUIManager* GetGameUIManager() const { return GameUIManager; }
    SLevelEditor* GetLevelEditor() const { return LevelEditor; }
    UnrealEd* GetUnrealEditor() const { return UnrealEditor; }

    FSlateAppMessageHandler* GetAppMessageHandler() const { return AppMessageHandler.get(); }
};
