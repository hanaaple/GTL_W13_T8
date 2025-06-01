// ReSharper disable CppClangTidyBugproneMacroParentheses
// ReSharper disable CppClangTidyClangDiagnosticPedantic
// ReSharper disable CppClangTidyClangDiagnosticReservedMacroIdentifier
#pragma once
#include <concepts>
#include "Class.h"
#include "ScriptStruct.h"
#include "UObjectHash.h"

// MSVC에서 매크로 확장 문제를 해결하기 위한 매크로
#define EXPAND_MACRO(x) x

// name을 문자열화 해주는 매크로
#define INLINE_STRINGIFY(name) #name

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)


// 공통 클래스 정의 부분
#define DECLARE_COMMON_CLASS_BODY(TClass, TSuperClass) \
private: \
    TClass(const TClass&) = delete; \
    TClass& operator=(const TClass&) = delete; \
    TClass(TClass&&) = delete; \
    TClass& operator=(TClass&&) = delete; \
    inline static struct TClass##_StaticClassRegistrar_PRIVATE \
    { \
        TClass##_StaticClassRegistrar_PRIVATE() \
        { \
            UClass::GetClassMap().Add(#TClass, ThisClass::StaticClass()); \
            AddClassToChildListMap(ThisClass::StaticClass()); \
        } \
    } TClass##_StaticClassRegistrar_PRIVATE{}; \
private: \
    static TMap<FString, std::function<void(sol::usertype<TClass>)>>& BindFunctions() { \
        static TMap<FString, std::function<void(sol::usertype<TClass>)>> _binds; \
        return _binds; \
    } \
public: \
    using InheritTypes = SolTypeBinding::InheritList<TClass, TSuperClass>::type; \
    static sol::usertype<TClass> GetLuaUserType(sol::state& lua) { \
        static sol::usertype<TClass> usertype = lua.new_usertype<TClass>( \
            #TClass, \
            sol::base_classes, \
            SolTypeBinding::TypeListToBases<typename SolTypeBinding::InheritList<TClass, TSuperClass>::base_list>::Get() \
        ); \
        return usertype; \
    } \
    static void BindPropertiesToLua(sol::state& lua) { \
        sol::usertype<TClass> table = GetLuaUserType(lua); \
        for (const auto [name, bind] : BindFunctions()) \
        { \
            bind(table); \
        } \
        SolTypeBinding::RegisterGetComponentByClass<AActor, UActorComponent, TClass>(lua, #TClass); \
    } \
public: \
    using Super = TSuperClass; \
    using ThisClass = TClass; \

// RTTI를 위한 클래스 매크로
#define DECLARE_CLASS(TClass, TSuperClass) \
    DECLARE_COMMON_CLASS_BODY(TClass, TSuperClass) \
    static UClass* StaticClass() { \
        static UClass ClassInfo{ \
            #TClass, \
            static_cast<uint32>(sizeof(TClass)), \
            static_cast<uint32>(alignof(TClass)), \
            TSuperClass::StaticClass(), \
            []() -> UObject* { \
                void* RawMemory = FPlatformMemory::AlignedMalloc<EAT_Object>(sizeof(TClass), alignof(TClass)); \
                ::new (RawMemory) TClass; \
                return static_cast<UObject*>(RawMemory); \
            }, \
            TClass::BindPropertiesToLua \
        }; \
        return &ClassInfo; \
    }

// RTTI를 위한 추상 클래스 매크로
#define DECLARE_ABSTRACT_CLASS(TClass, TSuperClass) \
    DECLARE_COMMON_CLASS_BODY(TClass, TSuperClass) \
    static UClass* StaticClass() { \
        static UClass ClassInfo{ \
            #TClass, \
            static_cast<uint32>(sizeof(TClass)), \
            static_cast<uint32>(alignof(TClass)), \
            TSuperClass::StaticClass(), \
            []() -> UObject* { return nullptr; }, \
            nullptr \
        }; \
        return &ClassInfo; \
    }


// ---------- DECLARE_STRUCT 관련 매크로 ----------
#define DECLARE_COMMON_STRUCT_BODY(TStruct, TSuperStruct) \
private: \
    inline static struct Z_##TStruct##_StructRegistrar_PRIVATE \
    { \
        Z_##TStruct##_StructRegistrar_PRIVATE() \
        { \
            UScriptStruct::GetScriptStructMap().Add(FName(INLINE_STRINGIFY(TStruct)), TStruct::StaticStruct()); \
        } \
    } Z_##TStruct##_StructRegistrar_Instance_PRIVATE{}; \
private: \
    static TMap<FString, std::function<void(sol::usertype<TStruct>)>>& BindFunctions() { \
        static TMap<FString, std::function<void(sol::usertype<TStruct>)>> _binds; \
        return _binds; \
    } \
public: \
    using Super = TSuperStruct; \
    using ThisClass = TStruct;

#define DECLARE_STRUCT_WITH_SUPER(TStruct, TSuperStruct) \
    DECLARE_COMMON_STRUCT_BODY(TStruct, TSuperStruct) \
    using InheritTypes = SolTypeBinding::InheritList<TStruct, TSuperStruct>::type; \
    static UScriptStruct* StaticStruct() \
    { \
        static_assert(std::derived_from<TStruct, TSuperStruct>, INLINE_STRINGIFY(TStruct) " must inherit from " INLINE_STRINGIFY(TSuperStruct)); \
        static UScriptStruct StructInfo{ \
            INLINE_STRINGIFY(TStruct), \
            static_cast<uint32>(sizeof(TStruct)), \
            static_cast<uint32>(alignof(TStruct)), \
            TSuperStruct::StaticStruct(), \
            TStruct::BindPropertiesToLua \
        }; \
        return &StructInfo; \
    } \
public: \
    static sol::usertype<TClass> GetLuaUserType(sol::state& lua) { \
        static sol::usertype<TClass> usertype = lua.new_usertype<TStruct>( \
            #TStruct, \
            sol::base_classes, \
            SolTypeBinding::TypeListToBases<typename SolTypeBinding::InheritList<TStruct, TSuperStruct>::base_list>::Get() \
        ); \
        return usertype; \
    } \
    static void BindPropertiesToLua(sol::state& lua) { \
        sol::usertype<TStruct> table = GetLuaUserType(lua); \
        for (const auto [name, bind] : BindFunctions()) \
        { \
            bind(table); \
        } \
    } \

#define DECLARE_STRUCT_NO_SUPER(TStruct) \
    DECLARE_COMMON_STRUCT_BODY(TStruct, TStruct) \
    using InheritTypes = SolTypeBinding::InheritList<TStruct, void>::type; \
    static UScriptStruct* StaticStruct() \
    { \
        static UScriptStruct StructInfo{ \
            INLINE_STRINGIFY(TStruct), \
            static_cast<uint32>(sizeof(TStruct)), \
            static_cast<uint32>(alignof(TStruct)), \
            nullptr, \
            TStruct::BindPropertiesToLua \
        }; \
        return &StructInfo; \
    } \
public: \
    static sol::usertype<TStruct> GetLuaUserType(sol::state& lua) { \
        static sol::usertype<TStruct> usertype = lua.new_usertype<TStruct>( \
            #TStruct \
        ); \
        return usertype; \
    } \
    static void BindPropertiesToLua(sol::state& lua) { \
        sol::usertype<TStruct> table = GetLuaUserType(lua); \
        for (const auto [name, bind] : BindFunctions()) \
        { \
            bind(table); \
        } \
    } \

#define GET_OVERLOADED_STRUCT_MACRO(_1, _2, MACRO, ...) MACRO

#define DECLARE_STRUCT(...) \
    EXPAND_MACRO(GET_OVERLOADED_STRUCT_MACRO(__VA_ARGS__, DECLARE_STRUCT_WITH_SUPER, DECLARE_STRUCT_NO_SUPER)(__VA_ARGS__))


// ---------- UProperty 관련 매크로 ----------
#define GET_FIRST_ARG(First, ...) First
#define FIRST_ARG(...) GET_FIRST_ARG(__VA_ARGS__, )

#define UPROPERTY_WITH_METADATA(InFlags, InMetadata, InType, InVarName, ...) \
    InType InVarName FIRST_ARG(__VA_ARGS__); \
    inline static struct InVarName##_PropRegistrar_PRIVATE \
    { \
        template <typename T> \
        void Bind() \
        { \
            if constexpr (T::value) { \
                static_assert((T::value) && (sizeof(InType) > 0), "Cannot use LuaReadWrite or LuaReadOnly with incomplete type"); \
                static_assert((T::value) && (sizeof(typename std::remove_pointer<InType>::type)) > 0, "Cannot use LuaReadWrite or LuaReadOnly with incomplete type"); \
                BindFunctions().Add(#InVarName, [](sol::usertype<ThisClass> table) { \
                    table[#InVarName] = &ThisClass::InVarName; \
                }); \
            } \
        } \
        InVarName##_PropRegistrar_PRIVATE() \
        { \
            constexpr int64 Offset = offsetof(ThisClass, InVarName); \
            constexpr EPropertyFlags Flags = InFlags; \
            UStruct* StructPtr = GetStructHelper<ThisClass>(); \
            StructPtr->AddProperty( \
                PropertyFactory::Private::MakeProperty<InType, Flags>( \
                    StructPtr, \
                    #InVarName, \
                    Offset, \
                    FPropertyMetadata InMetadata \
                ) \
            ); \
            constexpr bool LuaFlag = (Flags & LuaReadOnly) || (Flags & LuaReadWrite); \
            Bind<std::bool_constant<LuaFlag>>(); \
            StructPtr->Annotation.AddProperty(#InType, #InVarName); \
        } \
    } InVarName##_PropRegistrar_PRIVATE{};

#define UPROPERTY_WITH_FLAGS(InFlags, InType, InVarName, ...) \
    UPROPERTY_WITH_METADATA(InFlags, {}, InType, InVarName, __VA_ARGS__)

#define UPROPERTY_DEFAULT(InType, InVarName, ...) \
    UPROPERTY_WITH_FLAGS(EPropertyFlags::PropertyNone, InType, InVarName, __VA_ARGS__)

#define GET_OVERLOADED_PROPERTY_MACRO(_1, _2, _3, _4, _5, MACRO, ...) MACRO

/**
 * UClass에 Property를 등록합니다.
 *
 * ----- Example Code -----
 * 
 * UPROPERTY(int, Value)
 * 
 * UPROPERTY(int, Value, = 10)
 * 
 * UPROPERTY(EPropertyFlags::EditAnywhere, int, Value, = 10) // Flag를 지정하면 기본값은 필수
 * 
 * UPROPERTY(EPropertyFlags::EditAnywhere, ({ .Category="NewCategory", .DisplayName="MyValue" }), int, Value, = 10) // Metadata를 지정하면 Flag와 기본값은 필수
 *
 * @warning LuaReadOnly 또는 LuaReadWrite를 사용할 경우 불완전한 타입은 사용할 수 없습니다.
 */
#define UPROPERTY(...) \
    EXPAND_MACRO(GET_OVERLOADED_PROPERTY_MACRO(__VA_ARGS__, UPROPERTY_WITH_METADATA, UPROPERTY_WITH_FLAGS, UPROPERTY_DEFAULT, UPROPERTY_DEFAULT)(__VA_ARGS__))


// 오버로딩 고려해서 Id로 __COUNTER__를 받음
#define UFUNCTION_IMPL(Type, FuncName, Id, ...) \
    Type FuncName (__VA_ARGS__); \
    inline static struct CONCAT(FuncName##_PropRegister_, Id) \
    { \
        CONCAT(FuncName##_PropRegister_, Id)() \
        { \
            BindFunctions().Add(#FuncName, [](sol::usertype<ThisClass> table) { \
                table[#FuncName] = static_cast<Type (ThisClass::*)(__VA_ARGS__)>(&ThisClass::FuncName); \
            }); \
            UStruct* StructPtr = GetStructHelper<ThisClass>(); \
            StructPtr->Annotation.AddFunction(#Type, #FuncName, #__VA_ARGS__); \
        } \
    } CONCAT(FuncName##_PropRegister_Init_, Id) {}; \

/**
 * Lua에 Function을 바인딩합니다
 *
 * ----- Example Code -----
 *
 * UFUNCTION(const int, GetValue)
 *
 * UFUNCTION(bool, SetActorLocation, const FVector&)
 *
 * @warning 불완전한 타입은 사용할 수 없습니다.
 */
#define UFUNCTION(Type, FuncName, ...) \
    UFUNCTION_IMPL(Type, FuncName, __COUNTER__, __VA_ARGS__) \

#define UFUNCTION_CONST_IMPL(Type, FuncName, Id, ...) \
    Type FuncName (__VA_ARGS__) const; \
    inline static struct CONCAT(FuncName##_PropRegister_, Id) \
    { \
        CONCAT(FuncName##_PropRegister_, Id)() \
        { \
            BindFunctions().Add(#FuncName, [](sol::usertype<ThisClass> table) { \
                table[#FuncName] = static_cast<Type (ThisClass::*)(__VA_ARGS__) const>(&ThisClass::FuncName); \
            }); \
            UStruct* StructPtr = GetStructHelper<ThisClass>(); \
            StructPtr->Annotation.AddFunction(#Type, #FuncName, #__VA_ARGS__); \
        } \
    } CONCAT(FuncName##_PropRegister_Init_, Id) {}; \

/**
 * Lua에 const Function을 바인딩합니다
 *
 * ----- Example Code -----
 *
 * UFUNCTION_CONST(const int, GetValue)
 *
 * UFUNCTION_CONST(bool, SetActorLocation, const FVector&)
 *
 * @warning 불완전한 타입은 사용할 수 없습니다.
 */
#define UFUNCTION_CONST(Type, FuncName, ...) \
    UFUNCTION_CONST_IMPL(Type, FuncName, __COUNTER__, __VA_ARGS__) \
