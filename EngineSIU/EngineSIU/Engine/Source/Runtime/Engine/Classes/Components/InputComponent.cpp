#include "InputComponent.h"
#include "GameFramework/Actor.h"

void UInputComponent::ProcessKeyInput(float DeltaTime)
{
    uint8 keycode = static_cast<uint8>(EKeys::A);
    for (; keycode <= static_cast<uint8>(EKeys::Z); ++keycode)
    {
        unsigned char c = 'A' + keycode - static_cast<char>(EKeys::A);
        std::string s;
        s += c;
        if (PressedKeys.Contains(static_cast<EKeys::Type>(keycode)) && KeyBindDelegate.Contains(s))
        {
            KeyBindDelegate[s].Broadcast(DeltaTime);
        }
    }
}

void UInputComponent::ProcessMouseMoveInput(int dx, int dy)
{
}

void UInputComponent::SetPossess()
{
    if ( bIsPossess )
        return;

    BindInputDelegate();
    bIsPossess = true;
    //TODO: Possess일때 기존에 있던거 다시 넣어줘야할수도
}

void UInputComponent::BindInputDelegate()
{
    FSlateAppMessageHandler* Handler = GEngineLoop.GetAppMessageHandler();

    BindKeyDownDelegateHandles.Add(Handler->OnKeyDownDelegate.AddLambda([this](const FKeyEvent& InKeyEvent)
    {
        InputKey(InKeyEvent);
    }));

    BindKeyUpDelegateHandles.Add(Handler->OnKeyUpDelegate.AddLambda([this](const FKeyEvent& InKeyEvent)
    {
        InputKey(InKeyEvent);
    }));

    BindMouseMoveDelegateHandleId = Handler->OnRawMouseInputDelegate.AddLambda([this](const FPointerEvent& InMouseEvent)
    {
        FVector2D delta = InMouseEvent.GetCursorDelta();
        MouseMoveDelegate.Broadcast(delta.X, delta.Y);
    }).GetHandleId();
}

void UInputComponent::UnPossess()
{ 
    if ( !bIsPossess )
        return;

    ClearBindDelegate();
    bIsPossess = false;
}

void UInputComponent::ClearBindDelegate()
{
    FSlateAppMessageHandler* Handler = GEngineLoop.GetAppMessageHandler();

    for (FDelegateHandle DelegateHandle : BindKeyDownDelegateHandles)
    {
        Handler->OnKeyDownDelegate.Remove(DelegateHandle);
    }
     
    for (FDelegateHandle DelegateHandle : BindKeyUpDelegateHandles)
    {
        Handler->OnKeyUpDelegate.Remove(DelegateHandle);
    }

    Handler->OnRawMouseInputDelegate.Remove(FDelegateHandle(BindMouseMoveDelegateHandleId));
    
    BindKeyDownDelegateHandles.Empty();
    BindKeyUpDelegateHandles.Empty();
}

void UInputComponent::InputKey(const FKeyEvent& InKeyEvent)
{
    const EKeys::Type key = static_cast<EKeys::Type>(EKeys::A + (InKeyEvent.GetCharacter() - 'A'));
    if (InKeyEvent.GetInputEvent() == IE_Pressed)
    {
        PressedKeys.Add(key);
    }
    else if (InKeyEvent.GetInputEvent() == IE_Released)
    {
        PressedKeys.Remove(key);
    }
}


void UInputComponent::BeginPlay()
{
    UActorComponent::BeginPlay();
}

void UInputComponent::TickComponent(float DeltaTime)
{
    UActorComponent::TickComponent(DeltaTime);
    ProcessKeyInput(DeltaTime);
}

void UInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UActorComponent::EndPlay(EndPlayReason);
    UnPossess();
    UnBindAllAction();
}

uint64 UInputComponent::BindKeyAction(const FString& Key, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = KeyBindDelegate[Key].AddLambda([this, Callback](float DeltaTime)
    {
        Callback(DeltaTime);
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindTargetedKeyAction(const FString& Key, AActor* TargetObj, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }

    FDelegateHandle Handle = KeyBindDelegate[Key].AddWeakLambda(TargetObj, [this, Callback](float DeltaTime)
    {
        Callback(DeltaTime);
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindKeyLuaAction(const FString& Key, const sol::function Callback)
{
    if ( !Callback.valid() )
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = KeyBindDelegate[Key].AddLambda([this, Callback](float DeltaTime)
    {
        sol::protected_function_result res = Callback(DeltaTime);
        if (!res.valid())
        {
            sol::error err = res;
            UE_LOG(ELogLevel::Error, "Failed to execute event \n%s", err.what());
        }
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindTargetedKeyLuaAction(const FString& Key, AActor* TargetObj, const sol::function Callback)
{
    if ( !Callback.valid() )
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = KeyBindDelegate[Key].AddWeakLambda(TargetObj, [this, Callback](float DeltaTime)
    {
        sol::protected_function_result res = Callback(DeltaTime);
        if (!res.valid())
        {
            sol::error err = res;
            UE_LOG(ELogLevel::Error, "Failed to execute event \n%s", err.what());
        }
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

void UInputComponent::UnBindKeyAction(const FString& Key, uint64 HandleId)
{
    BoundActionHandleIds.Remove(HandleId);
    KeyBindDelegate[Key].Remove(FDelegateHandle(HandleId));
}

uint64 UInputComponent::BindMouseMoveAction(const std::function<void(int dx, int dy)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = MouseMoveDelegate.AddLambda([Callback](int dx, int dy)
    {
        Callback(dx, dy);
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindTargetedMouseMoveAction(AActor* TargetObj, const std::function<void(int dx, int dy)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }

    FDelegateHandle Handle = MouseMoveDelegate.AddWeakLambda(TargetObj, [Callback](int dx, int dy)
    {
        Callback(dx, dy);
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindMouseMoveLuaAction(const sol::function Callback)
{
    if ( !Callback.valid() )
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = MouseMoveDelegate.AddLambda([this, Callback](int dx, int dy)
    {
        sol::protected_function_result res = Callback(dx, dy);
        if (!res.valid())
        {
            sol::error err = res;
            UE_LOG(ELogLevel::Error, "Failed to execute event \n%s", err.what());
        }
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

uint64 UInputComponent::BindTargetedMouseMoveLuaAction(AActor* TargetObj, const sol::function Callback)
{
    if ( !Callback.valid() )
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = MouseMoveDelegate.AddWeakLambda(TargetObj, [this, Callback](int dx, int dy)
    {
        sol::protected_function_result res = Callback(dx, dy);
        if (!res.valid())
        {
            sol::error err = res;
            UE_LOG(ELogLevel::Error, "Failed to execute event \n%s", err.what());
        }
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

void UInputComponent::UnBindMouseMoveAction(uint64 HandleId)
{
    BoundActionHandleIds.Remove(HandleId);
    MouseMoveDelegate.Remove(FDelegateHandle(HandleId));
}

void UInputComponent::UnBindAllAction()
{
    for (auto& [name, Delegate]: KeyBindDelegate)
    {
        for (const uint64 id: BoundActionHandleIds)
        {
            Delegate.Remove(FDelegateHandle(id));
        }
    }

    for (const uint64 id: BoundActionHandleIds)
    {
        MouseMoveDelegate.Remove(FDelegateHandle(id));
    }
    
    BoundActionHandleIds.Empty();
}
