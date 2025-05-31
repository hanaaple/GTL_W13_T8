#pragma once
#include "NameTypes.h"
#include "Container/Map.h"

class UObject;

struct FDuplicateParams
{
    UObject*    Source = nullptr;
    UObject*    DestOuter = nullptr;
    FName       DestName = NAME_None;
    bool        bSkipOuterDuplication = true;  // Outer 객체는 얕은 참조
};

class FObjectDuplicator
{
    FDuplicateParams Params;
    TMap<UObject*, UObject*> DuplicatedMap;
public:
    FObjectDuplicator(const FDuplicateParams& In) : Params(In) {}

    UObject* DuplicateObject(UObject* Src);
};
