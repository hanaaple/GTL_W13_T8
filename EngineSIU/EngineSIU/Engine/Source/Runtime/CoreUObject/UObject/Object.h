#pragma once
#include "EngineLoop.h"
#include "NameTypes.h"
#include "ScriptBind.h"
#include "Misc/CoreMiscDefines.h"

class FObjectDuplicator;
struct FPropertyChangedEvent;
extern FEngineLoop GEngineLoop;

class UClass;
class UWorld;
class AActor;
class UActorComponent;

namespace SolTypeBinding
{
    // inline static TMap<FString, sol::table> RegisteredComponentType;

    // Register to AActor::GetComponentByClass
    // IsCompleteType_v<T>의 인스턴스화 위해 AActor, UActorComponent 역시 템플릿으로 받음.
    template <typename A, typename C, typename T>
    void RegisterGetComponentByClass(sol::state& lua, std::string className)
    {
        if constexpr ( IsCompleteType_v<A> && IsCompleteType_v<C> && std::derived_from<T, C> )
        {
            {
                auto funcPtr = static_cast<T* (A::*)() const>(&A::template GetComponentByClass<T>);
                A::GetLuaUserType(lua)["Get" + className] = funcPtr;
                A::StaticClass()->Annotation.AddFunction(className, "Get" + className, "");
            }
            {
                sol::usertype<TArray<T*>> typeTable = lua.new_usertype<TArray<T*>>(className+"Table");
                typeTable["GetContainer"] = static_cast<typename TArray<T*>::ArrayType& (TArray<T*>::*)()>(&TArray<T*>::GetContainerPrivate);
                auto funcPtr = static_cast<TArray<T*> (A::*)() const>(&A::template GetComponentsByClass<T>);
                A::GetLuaUserType(lua)["Get" + className + "s"] = funcPtr;
                A::StaticClass()->Annotation.AddFunction(className + "Table", "Get" + className + "s", "");
            }
            
            UE_LOG(ELogLevel::Display, "Register AActor::Get%s", className.c_str());
        } 
    }
}

class UObject
{
    friend class AActor;
private:
    UObject(const UObject&) = delete;
    UObject& operator=(const UObject&) = delete;
    UObject(UObject&&) = delete;
    UObject& operator=(UObject&&) = delete;

public:
    using InheritTypes = SolTypeBinding::InheritList<UObject>::type;
    using Super = UObject;
    using ThisClass = UObject;

    static UClass* StaticClass();

private:
    friend class FObjectFactory;
    friend class FSceneMgr;
    friend class UStruct;
    friend class UClass;

    uint32 UUID;
    uint32 InternalIndex; // Index of GUObjectArray

    FName NamePrivate;
    UClass* ClassPrivate = nullptr;
    UObject* OuterPrivate = nullptr;

    // FName을 키값으로 넣어주는 컨테이너를 모두 업데이트 해야합니다.
    void SetFName(const FName& InName) { NamePrivate = InName; }

public:
    UObject();
    virtual ~UObject() = default;

    UObject* Duplicate(UObject* InOuter);

    /**
     * 이 객체의 프로퍼티 중 하나가 에디터 등 외부 요인에 의해 변경된 후 호출됩니다.
     * @param PropertyChangedEvent 변경된 프로퍼티에 대한 정보를 담고 있습니다.
     */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

    UObject* GetOuter() const { return OuterPrivate; }
    virtual UWorld* GetWorld() const;
    virtual void Serialize(FArchive& Ar);

    FName GetFName() const { return NamePrivate; }
    FString GetName() const { return NamePrivate.ToString(); }


    uint32 GetUUID() const { return UUID; }
    uint32 GetInternalIndex() const { return InternalIndex; }

    UClass* GetClass() const { return ClassPrivate; }

    /** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
    bool IsA(const UClass* SomeBase) const;

    template <typename T>
        // requires std::derived_from<T, UObject>
    bool IsA() const
    {
        return IsA(T::StaticClass());
    }


    virtual void DisplayProperty() {}    
    // UObjectBaseUtility

    /** 이 Object를 삭제 대기열에 추가합니다. */
    void MarkAsGarbage();

    // ~UObjectBaseUtility

public:
    FVector4 EncodeUUID() const
    {
        FVector4 Result;

        Result.X = static_cast<float>(UUID % 0xFF);
        Result.Y = static_cast<float>(UUID >> 8 & 0xFF);
        Result.Z = static_cast<float>(UUID >> 16 & 0xFF);
        Result.W = static_cast<float>(UUID >> 24 & 0xFF);

        return Result;
    }

    virtual void SerializeAsset(FArchive& Ar) {}
    virtual void PostDuplicate();
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator);

};
