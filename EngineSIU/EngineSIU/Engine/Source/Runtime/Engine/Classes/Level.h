#pragma once
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class AActor;
class UWorld;


class ULevel : public UObject
{
    DECLARE_CLASS(ULevel, UObject)

public:
    ULevel() = default;

    void InitLevel(UWorld* InOwningWorld);
    void Release();
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

    //virtual UObject* Duplicate(UObject* InOuter) override;

    TArray<AActor*> Actors;
    UPROPERTY(UWorld*, OwningWorld, = nullptr)
};
