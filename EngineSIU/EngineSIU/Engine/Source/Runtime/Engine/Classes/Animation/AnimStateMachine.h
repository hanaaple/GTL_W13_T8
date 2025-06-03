#pragma once
#include<UObject/Object.h>
#include "UObject/ObjectMacros.h"


class UAnimStateMachine : public UObject
{
    DECLARE_CLASS(UAnimStateMachine, UObject)

    public:
    UAnimStateMachine();
    virtual ~UAnimStateMachine() override = default;
    void MoveFast();
    void MoveSlow();
    
    FString GetState() const;
    
private:
    FString State;
    int MoveSpeed;
};
