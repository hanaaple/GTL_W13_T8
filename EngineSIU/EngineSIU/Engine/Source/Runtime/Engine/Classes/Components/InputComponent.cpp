#include "InputComponent.h"
#include "GameFramework/Actor.h"

void UInputComponent::ProcessInput(float DeltaTime)
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

void UInputComponent::SetPossess()
{
    BindInputDelegate();
    
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
    
}

void UInputComponent::UnPossess()
{ 
    ClearBindDelegate();
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
    ProcessInput(DeltaTime);
}

void UInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UActorComponent::EndPlay(EndPlayReason);
    UnPossess();
    UnBindAllAction();
}

uint64 UInputComponent::BindAction(const FString& Key, const std::function<void(float)>& Callback)
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

uint64 UInputComponent::BindTargetedAction(const FString& Key, AActor* TargetObj, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }

    // TODO: lua script 함수를 람다로 넘겨줬다 에러나면 Panic나면서 터진다. 람다 말고 다른 대안 없을까...
    FDelegateHandle Handle = KeyBindDelegate[Key].AddWeakLambda(TargetObj, [this, Callback](float DeltaTime)
    {
        Callback(DeltaTime);
    });
    BoundActionHandleIds.Add(Handle.GetHandleId());
    return Handle.GetHandleId();
}

void UInputComponent::UnBindAction(const FString& Key, uint64 HandleId)
{
    BoundActionHandleIds.Remove(HandleId);
    KeyBindDelegate[Key].Remove(FDelegateHandle(HandleId));
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
    BoundActionHandleIds.Empty();
}
