#pragma once
#include <filesystem>

#include "Container/Array.h"
#include "Container/Map.h"
#include "Container/String.h"
#include "sol/sol.hpp"

class AActor;

struct FLuaScriptInfo
{
    std::string FileName;   // 스크립트 파일명
    std::string Source;     // 스크립트 내용
    std::filesystem::file_time_type LastModifiedTime;
};

struct FLuaScriptManager
{
public:
    void Initialize();
    sol::state& GetState();
    sol::environment& GetSharedEnvironment();

    void BindTypes();
    void InitPIEScripts(TArray<AActor*>& Actors);
    
    void Reload();
    void ReloadForce();

    FString GetBasePath() { return BasePath; }

    TMap<FString, FLuaScriptInfo> LuaScripts;
    
    static std::string ToString(const sol::object& obj, int depth = 0, bool showHidden = false);
private:
    sol::state LuaState;
    sol::environment SharedEnvironment;

    void BindPrimitiveTypes();
    void BindUObject();
    void BindStructs();
    void UpdateStub();
    bool LoadFile(const FString& FileName);

    bool IsFileOutdated(const FString& FileName);

    FString BasePath = FString(L"LuaScripts");
    FString StubBasePath = FString(L"LuaScriptsStubs");

    static void PanicHandler(sol::optional<std::string> MaybeMsg);
};
