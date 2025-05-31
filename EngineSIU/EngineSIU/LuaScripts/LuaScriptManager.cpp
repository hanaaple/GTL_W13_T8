#include "LuaScriptManager.h"

#include <fstream>

#include "Container/Array.h"
#include "UserInterface/Console.h"

void FLuaScriptManager::Initialize()
{
    LuaState.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );

    LuaState["SCRIPT_BASE_PATH"] = BasePath;
    SharedEnvironment = sol::environment(LuaState, sol::create, LuaState.globals());
}

sol::state& FLuaScriptManager::GetState()
{
    return LuaState;
}

sol::environment& FLuaScriptManager::GetSharedEnvironment()
{
    return SharedEnvironment;
}

void FLuaScriptManager::Reload()
{
    TArray<FString> FilePaths;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(GetData(BasePath)))
    {
        FilePaths.Add(entry.path().string());
    }

    for (const auto& path: FilePaths)
    {
        if (IsFileOutdated(path))
        {
            LoadFile(path);
            // for (const auto iter : TObjectRange<UScriptableComponent>())
            // {
            //     iter->LoadScriptAndBind();
            // }
        }
    }
}

void FLuaScriptManager::ReloadForce()
{
    TArray<FString> FilePaths;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(GetData(BasePath)))
    {
        FilePaths.Add(entry.path().string());
    }

    for (const auto& path: FilePaths)
    {
        if (IsFileOutdated(path))
        {
            LoadFile(path);
            // for (const auto iter : TObjectRange<UScriptableComponent>())
            // {
            //     iter->InitEnvironment();
            //     iter->LoadScriptAndBind();
            // }
        }
    }
}

void FLuaScriptManager::BindPrimitiveTypes()
{
}

void FLuaScriptManager::BindUObject()
{
}

bool FLuaScriptManager::LoadFile(const FString& FileName)
{
    std::string FileNameStr = GetData(FileName);
    std::ifstream file(FileNameStr);
    if (!file.is_open())
    {
        // Cannot find File
        UE_LOG(ELogLevel::Error, "Failed to open %s", FileNameStr.c_str());
        return false;
    }

    LuaScriptInfo scriptInfo;
    std::stringstream buffer;
    buffer << file.rdbuf();
    scriptInfo.Source = buffer.str();
    scriptInfo.LastModifiedTime = std::filesystem::last_write_time(FileNameStr);
    
    sol::load_result res = LuaState.load_buffer(scriptInfo.Source.c_str(), scriptInfo.Source.size());
    if (!res.valid())
    {
        // Cannot compile Source
        sol::error err = res;
        UE_LOG(ELogLevel::Error, "Failed to open %s", err.what());
        return false;
    }
    return true;
}

bool FLuaScriptManager::IsFileOutdated(const FString& FileName)
{
    std::string FileNameStr = GetData(FileName);
    if (!std::filesystem::exists(FileNameStr))
        return false;
    
    auto currentTime = std::filesystem::last_write_time(FileNameStr);
    std::filesystem::file_time_type lastModifiedTime;
    for (const auto& [name, scriptInfo]: LuaScripts)
    {
        if (scriptInfo.FileName == FileNameStr)
            lastModifiedTime = scriptInfo.LastModifiedTime;
    }
    // 만약 메인 파일의 저장된 타임스탬프가 없거나 현재와 다르면 변경된 것으로 처리
    if (lastModifiedTime != currentTime)
        return true;
        
    return false;
}
