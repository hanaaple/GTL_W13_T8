#include "Object.h"

#include "ObjectFactory.h"
#include "Class.h"
#include "FObjectDuplicator.h"
#include "Engine/Engine.h"


UClass* UObject::StaticClass()
{
    static UClass ClassInfo{
        "UObject",
        sizeof(UObject),
        alignof(UObject),
        nullptr,
        []() -> UObject*
        {
            void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(sizeof(UObject));
            ::new (RawMemory) UObject;
            return static_cast<UObject*>(RawMemory);
        },
        [](sol::state&) {}
    };
    return &ClassInfo;
}

UObject::UObject()
    : UUID(0)
    // TODO: Object를 생성할 때 직접 설정하기
    , InternalIndex(-1)
    , NamePrivate("None")
{
}

UObject* UObject::Duplicate(UObject* InOuter)
{
    //return FObjectFactory::ConstructObject(GetClass(), InOuter);
    // 복제 파라미터 설정
    FDuplicateParams Params;
    Params.Source     = this;                          // 원본 액터(this)
    Params.DestOuter  = InOuter;                       // 새 Outer (예: 스폰된 월드)
    Params.DestName   = TEXT("CopyOf_") + GetName();   // 새 이름

    // 복제 실행
    FObjectDuplicator Duplicator(Params);
    return Duplicator.DuplicateObject(this);
}

void UObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}

void UObject::Serialize(FArchive& Ar)
{
    // TODO: Serialize 구현
    // GetClass()->SerializeBin(Ar, this);
}

UWorld* UObject::GetWorld() const
{
    if (UObject* Outer = GetOuter())
    {
        return Outer->GetWorld();
    }
    return nullptr;
}

void UObject::MarkAsGarbage()
{
    GUObjectArray.MarkRemoveObject(this);
}

void UObject::PostDuplicate()
{
}

void UObject::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
}

bool UObject::IsA(const UClass* SomeBase) const
{
    const UClass* ThisClass = GetClass();
    return ThisClass->IsChildOf(SomeBase);
}
