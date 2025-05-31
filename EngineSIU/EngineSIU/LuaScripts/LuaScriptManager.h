#pragma once
#include <filesystem>

#include "Container/Map.h"
#include "Container/String.h"
#include "sol/sol.hpp"

struct LuaScriptInfo
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
    
    void Reload();
    void ReloadForce();

    TMap<FString, LuaScriptInfo> LuaScripts;
private:
    sol::state LuaState;
    sol::environment SharedEnvironment;

    void BindPrimitiveTypes();
    void BindUObject();
    void BindStructs();
    bool LoadFile(const FString& FileName);

    bool IsFileOutdated(const FString& FileName);

    FString BasePath = FString(L"LuaScripts");
};
