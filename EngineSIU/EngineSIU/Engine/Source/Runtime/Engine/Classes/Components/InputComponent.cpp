#include "InputComponent.h"
#include "GameFramework/Actor.h"

void UInputComponent::ProcessInput(float DeltaTime)
{
    if (PressedKeys.Contains(EKeys::W))
    {
        KeyBindDelegate[FString("W")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::A))
    {
        KeyBindDelegate[FString("A")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::S))
    {
        KeyBindDelegate[FString("S")].Broadcast(DeltaTime);
    }
    if (PressedKeys.Contains(EKeys::D))
    {
        KeyBindDelegate[FString("D")].Broadcast(DeltaTime);
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
    // 일반적인 단일 키 이벤트
    switch (InKeyEvent.GetCharacter())
    {
    case 'W':
        {
            if (InKeyEvent.GetInputEvent() == IE_Pressed)
            {
                PressedKeys.Add(EKeys::W);
            }
            else if (InKeyEvent.GetInputEvent() == IE_Released)
            {
                PressedKeys.Remove(EKeys::W);
            }
            break;
        }
    case 'A':
        {
            if (InKeyEvent.GetInputEvent() == IE_Pressed)
            {
                PressedKeys.Add(EKeys::A);
            }
            else if (InKeyEvent.GetInputEvent() == IE_Released)
            {
                PressedKeys.Remove(EKeys::A);
            }
            break;
        }
    case 'S':
        {
            if (InKeyEvent.GetInputEvent() == IE_Pressed)
            {
                PressedKeys.Add(EKeys::S);
            }
            else if (InKeyEvent.GetInputEvent() == IE_Released)
            {
                PressedKeys.Remove(EKeys::S);
            }
            break;
        }
    case 'D':
        {
            if (InKeyEvent.GetInputEvent() == IE_Pressed)
            {
                PressedKeys.Add(EKeys::D);
            }
            else if (InKeyEvent.GetInputEvent() == IE_Released)
            {
                PressedKeys.Remove(EKeys::D);
            }
            break;
        }
    default:
        break;
    }
}


void UInputComponent::BindAction(const FString& Key, const std::function<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return;
    }
    
    KeyBindDelegate[Key].AddLambda([this, Callback](float DeltaTime)
    {
        Callback(DeltaTime);
    });
}

uint64 UInputComponent::BindLuaAction(const FString& Key, AActor* LuaObj, const TFunction<void(float)>& Callback)
{
    if (Callback == nullptr)
    {
        return UINT64_MAX;
    }
    
    FDelegateHandle Handle = KeyBindDelegate[Key].AddWeakLambda(LuaObj, [this, Callback](float DeltaTime)
    {
        Callback(DeltaTime);
    });

    return Handle.GetHandleId();
}

void UInputComponent::UnBindLuaAction(const FString& Key, uint64 HandleId)
{
    KeyBindDelegate[Key].Remove(FDelegateHandle(HandleId));
}
