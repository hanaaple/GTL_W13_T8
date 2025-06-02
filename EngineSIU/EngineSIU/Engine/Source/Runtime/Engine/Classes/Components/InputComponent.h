#pragma once

#include "Core/Container/Map.h"
#include "Delegates/DelegateCombination.h"
#include "Runtime/InputCore/InputCoreTypes.h"
#include "Components/ActorComponent.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOneFloatDelegate, const float&)
DECLARE_MULTICAST_DELEGATE_TwoParams(FMouseMoveDelegate, const int&, const int&)

class UInputComponent : public UActorComponent
{
    DECLARE_CLASS(UInputComponent, UActorComponent)

public:
    UInputComponent() = default;
    virtual ~UInputComponent() override = default;
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    UFUNCTION(uint64, BindKeyAction, const FString& Key, const std::function<void(float)>& Callback)
    UFUNCTION(uint64, BindTargetedKeyAction, const FString& Key, AActor* TargetObj, const std::function<void(float)>& Callback)
    UFUNCTION(void, UnBindKeyAction, const FString& Key, uint64 HandleId)

    UFUNCTION(uint64, BindMouseMoveAction, const std::function<void(int, int)>& Callback)
    UFUNCTION(uint64, BindTargetedMouseMoveAction, AActor* TargetObj, const std::function<void(int, int)>& Callback)
    UFUNCTION(void, UnBindMouseMoveAction, uint64 HandleId)
    
    void ProcessKeyInput(float DeltaTime);
    void ProcessMouseMoveInput(int dx, int dy);
    
    UFUNCTION(void, SetPossess)
    UFUNCTION(void, UnPossess)
    void BindInputDelegate();
    void ClearBindDelegate();
    // Possess가 풀렸다가 다시 왔을때 원래 바인딩 돼있던 애들 일괄적으로 다시 바인딩해줘야할수도 있음.
    void InputKey(const FKeyEvent& InKeyEvent);

private:
    TMap<FString, FOneFloatDelegate> KeyBindDelegate;
    TArray<FDelegateHandle> BindKeyDownDelegateHandles;
    TArray<FDelegateHandle> BindKeyUpDelegateHandles;
    
    FMouseMoveDelegate MouseMoveDelegate;
    uint64 BindMouseMoveDelegateHandleId;
    
    TArray<uint64> BoundActionHandleIds;
    TSet<EKeys::Type> PressedKeys;

    void UnBindAllAction();
    bool bIsPossess = false;
};
