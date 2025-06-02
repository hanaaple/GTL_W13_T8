#pragma once
#include "Struct.h"


/**
 * C++ DECLARE_STRUCT() 매크로로 정의된 사용자 정의 구조체에 대한 메타데이터를 나타냅니다.
 */
class UScriptStruct : public UStruct
{
public:
    UScriptStruct(
        const char* InName,
        uint32 InStructSize,
        uint32 InAlignment,
        UScriptStruct* InSuperScriptStruct, // C++ 구조체도 상속 가능 (선택적, 보통은 nullptr)
        std::function<void(sol::state&)> InBindScript = nullptr
    );

    virtual ~UScriptStruct() override = default;

    UScriptStruct(const UScriptStruct&) = delete;
    UScriptStruct& operator=(const UScriptStruct&) = delete;
    UScriptStruct(UScriptStruct&&) = delete;
    UScriptStruct& operator=(UScriptStruct&&) = delete;

    using Super = UStruct;
    using ThisClass = UScriptStruct;

public:
    // 전역 ScriptStruct 맵 접근
    static TMap<FName, UScriptStruct*>& GetScriptStructMap();

    // 이름으로 ScriptStruct 찾기
    static UScriptStruct* FindScriptStruct(const FName& StructName);
    
    /** Lua에 UPROPERTY를 Bind하는 함수.
     *  DECLARE_STRUCT에서 초기화됨
     */
    const std::function<void(sol::state&)> BindPropertiesToLua;
};
