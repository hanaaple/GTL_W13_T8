#pragma once
#include "EngineLoop.h"
#include "NameTypes.h"
#include "sol/sol.hpp"
#include "Misc/CoreMiscDefines.h"

struct FPropertyChangedEvent;
extern FEngineLoop GEngineLoop;

class UClass;
class UWorld;
class AActor;
class UActorComponent;

namespace SolTypeBinding
{
    template<typename... Types>
    struct TypeList {};

    // PushBack
    template<typename List, typename NewType>
    struct PushBack;

    // Pushback
    template<typename... Types, typename NewType>
    struct PushBack<TypeList<Types...>, NewType> {
        using type = TypeList<Types..., NewType>;
    };

    // Base 클래스를 상속하는 모든 타입을 리스트로 모은다
    template<typename Derived, typename Base = void>
    struct InheritList;

    // Base 없는 경우 (Root 클래스)
    template<typename Derived>
    struct InheritList<Derived, void> {
        using type = TypeList<Derived>;
    };

    // Base 있는 경우 (Derived -> Base)
    template<typename Derived, typename Base>
    struct InheritList {
        using base_list = typename Base::InheritTypes;
        using type = typename PushBack<base_list, Derived>::type;
    };

    // to unpack types
    template<typename TypeList>
    struct TypeListToBases;

    // to unpack types
    template<typename... Types>
    struct TypeListToBases<TypeList<Types...>> {
        static auto Get() {
            return sol::bases<Types...>();
        }
    };

    // for Register to AActor::GetComponentByClass
    template <typename T>
    constexpr bool IsCompleteType_v = requires { sizeof(T); };
    
    // Register to AActor::GetComponentByClass
    template <typename T>
    void RegisterGetComponentByClass(sol::state& lua, std::string className)
    {
        if constexpr ( IsCompleteType_v<AActor> && IsCompleteType_v<UActorComponent> && std::derived_from<T, UActorComponent> )
        {
            // 암시적 형변환에서 AActor가 완전한 타입임을 요구해서 명시적으로 형변환.
            using FuncType = T* (AActor::*)();
            auto funcPtr = static_cast<FuncType>(&AActor::template GetComponentByClass<T>);
            AActor::GetLuaUserType(lua)["Get" + className] = funcPtr;
            std::cout << "Register AActor::Get" << className << std::endl;
        } else
        {
            std::cout << "Failed Register AActor::Get" << className << std::endl;
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

    virtual UObject* Duplicate(UObject* InOuter);

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
};
