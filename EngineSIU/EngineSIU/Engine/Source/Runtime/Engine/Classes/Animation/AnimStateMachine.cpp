#include "AnimStateMachine.h"

UAnimStateMachine::UAnimStateMachine()
{
    State = "None";
    MoveSpeed = 0;
}

FString UAnimStateMachine::GetState() const
{
    return State;
}

void UAnimStateMachine::MoveFast()
{
    MoveSpeed++;
    MoveSpeed = FMath::Clamp(MoveSpeed, 0, 3);
}

void UAnimStateMachine::MoveSlow()
{
    MoveSpeed--;
    MoveSpeed = FMath::Clamp(MoveSpeed, 0, 3);
}

