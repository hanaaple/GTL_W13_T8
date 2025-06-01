#pragma once
#include<UObject/Object.h>
#include "UObject/ObjectMacros.h"

enum EAnimState
{
    AS_Idle,
    AS_Dance,
    AS_SlowRun,
    AS_NarutoRun,
    AS_FastRun,
};
class UAnimStateMachine : public UObject
{
    DECLARE_CLASS(UAnimStateMachine, UObject)

    public:
    UAnimStateMachine();
    virtual ~UAnimStateMachine() override = default;
    void ProcessState();
    void MoveFast();
    void MoveSlow();
    void Dance();
    void StopDance();
    
    FString GetStateName(EAnimState State) const;
    EAnimState GetState() const;
    
private:
    EAnimState CurrentState;
    int MoveSpeed;
    bool bIsDancing;
};
