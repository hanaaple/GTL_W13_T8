#include "LuaScriptManager.h"

#include <fstream>

#include "Container/Array.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "UObject/ScriptStruct.h"
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
    
    BindTypes();
}

sol::state& FLuaScriptManager::GetState()
{
    return LuaState;
}

sol::environment& FLuaScriptManager::GetSharedEnvironment()
{
    return SharedEnvironment;
}

void FLuaScriptManager::BindTypes()
{
    BindPrimitiveTypes();
    BindUObject();
    BindStructs();
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
    using mFunc = sol::meta_function;
    
    // FVector
    sol::usertype<FVector> vectorTypeTable = LuaState.new_usertype<FVector>("FVector",
        sol::call_constructor,
        sol::constructors<FVector(), FVector(float, float, float)>()
    );

    vectorTypeTable["x"] = &FVector::X;
    vectorTypeTable["y"] = &FVector::Y;
    vectorTypeTable["z"] = &FVector::Z;
    vectorTypeTable["Dot"] = &FVector::Dot;
    vectorTypeTable["Cross"] = &FVector::Cross;
    vectorTypeTable["Length"] = &FVector::Length;
    vectorTypeTable["SquaredLength"] = &FVector::SquaredLength;
    vectorTypeTable["Distance"] = &FVector::Distance;
    vectorTypeTable["Normal"] = &FVector::GetSafeNormal;
    vectorTypeTable["Normalize"] = &FVector::Normalize;
    
    vectorTypeTable[mFunc::addition] = [](const FVector& a, const FVector& b) { return a + b; };
    vectorTypeTable[mFunc::subtraction] = [](const FVector& a, const FVector& b) { return a - b; };
    vectorTypeTable[mFunc::multiplication] = [](const FVector& v, const float f) { return v * f; };
    vectorTypeTable[mFunc::equal_to] = [](const FVector& a, const FVector& b) { return a == b; };

    // FRotator
    sol::usertype<FRotator> rotatorTypeTable = LuaState.new_usertype<FRotator>("FRotator",
        sol::call_constructor,
        sol::constructors<FRotator(), FRotator(float, float, float)>()
    );
    
    rotatorTypeTable["Pitch"] = &FRotator::Pitch;
    rotatorTypeTable["Yaw"] = &FRotator::Yaw;
    rotatorTypeTable["Roll"] = &FRotator::Roll;
    rotatorTypeTable["ClampAxis"] = &FRotator::ClampAxis;
    rotatorTypeTable["GetNormalized"] = &FRotator::GetNormalized;
    
    rotatorTypeTable[mFunc::addition] = [](const FRotator& a, const FRotator& b) { return a + b; };
    rotatorTypeTable[mFunc::subtraction] = [](const FRotator& a, const FRotator& b) { return a - b; };
    rotatorTypeTable[mFunc::multiplication] = [](const FRotator& v, const float f) { return v * f; };
    rotatorTypeTable[mFunc::equal_to] = [](const FRotator& a, const FRotator& b) { return a == b; };
    
    // FString
    sol::usertype<FString> stringTypeTable = LuaState.new_usertype<FString>("FString",
        sol::call_constructor,
        sol::constructors<FString(), FString(const std::string&), FString(const ANSICHAR*)>()
    );
    
    stringTypeTable["ToBool"] = &FString::ToBool;
    stringTypeTable["ToFloat"] = &FString::ToFloat;
    stringTypeTable["ToInt"] = &FString::ToInt;
    stringTypeTable["ToString"] = [](const FString& String) -> const char* { return GetData(String); };
    stringTypeTable["Len"] = &FString::Len;
    stringTypeTable["IsEmpty"] = &FString::IsEmpty;

    stringTypeTable[mFunc::addition] = [](const FString& a, const FString& b) { return a + b; };
    stringTypeTable[mFunc::equal_to] = [](const FString& a, const FString& b) { return a == b; }; 
}

void FLuaScriptManager::BindUObject()
{
    sol::usertype<UObject> UObjectTypeTable = LuaState.new_usertype<UObject>("UObject");
    UObjectTypeTable["GetUUID"] = &UObject::GetUUID;
    UObjectTypeTable["GetName"] = &UObject::GetName;
    UObjectTypeTable["GetClass"] = &UObject::GetClass;
    UObjectTypeTable["IsA"] = static_cast<bool (UObject::*)(const UClass*) const>(&UObject::IsA); // 오버라이드 함수 명시화.

    for (const auto& [name, type]: UClass::GetClassMap())
    {
        if (type->BindPropertiesToLua != nullptr)
        {
            UE_LOG(ELogLevel::Display, "Binding Script %s", GetData(name.ToString()));
            type->BindPropertiesToLua(LuaState);
        }
    }
}

void FLuaScriptManager::BindStructs()
{
    for (const auto& [name, type]: UScriptStruct::GetScriptStructMap())
    {
        if (type->BindPropertiesToLua != nullptr)
        {
            UE_LOG(ELogLevel::Display, "Binding Script %s", GetData(name.ToString()));
            type->BindPropertiesToLua(LuaState);
        }
    }
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
