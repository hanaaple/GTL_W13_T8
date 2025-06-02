#include "LuaScriptManager.h"

#include <fstream>

#include "LuaScriptComponent.h"
#include "Container/Array.h"
#include "Engine/Engine.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "UObject/ScriptStruct.h"
#include "UObject/UObjectIterator.h"
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
    
    lua_atpanic(LuaState, sol::c_call<decltype(&FLuaScriptManager::PanicHandler), &FLuaScriptManager::PanicHandler>);

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

    // Print
    LuaState["ToString"] = &FLuaScriptManager::ToString;
    LuaState["PrintObj"] = [](const sol::object& obj) {UE_LOG(ELogLevel::Display, "%s", ToString(obj).c_str());};
    LuaState["LogDisplay"] = [](const std::string& str) {UE_LOG(ELogLevel::Display, "%s", str.c_str());};
    LuaState["LogWarning"] = [](const std::string& str) {UE_LOG(ELogLevel::Warning, "%s", str.c_str());};
    LuaState["LogError"] = [](const std::string& str) {UE_LOG(ELogLevel::Error, "%s", str.c_str());};

    LuaState.set_function("SpawnActor", [&](const std::string& className, sol::optional<std::string> luaActorName) -> AActor* 
    {
        UWorld* World = GEngine->ActiveWorld;
        // 문자열 FName으로 변환
        FString ClassName = className;
        FName fnClassName(ClassName);

        // UClass* 검색
        UClass* cls = UClass::FindClass(fnClassName);
        if (!cls) {
            // 존재하지 않는 클래스 이름인 경우
            return nullptr;
        }

        FName fnActorName = luaActorName ? FName{luaActorName->c_str()}
        : FName{}; // 기본 생성자는 none

        //액터 스폰
        return World->SpawnActor(cls, fnActorName);
    });
    
    LuaState.set_function("GetActorByName", [&](const std::string& str) -> AActor*
    {
        TArray<AActor*> Actors = GEngine->ActiveWorld->GetActiveLevel()->Actors;
        for (AActor* Actor: Actors)
        {
            // TODO: PIE로 복사됐을 때 Original의 Label을 찾아가야 하나?
            if (Actor->GetDefaultActorLabel() == FString(str))
            {
                return Actor;
            }
        }
        return nullptr;
    });

    LuaState["KINDA_SMALL_NUMBER"] = KINDA_SMALL_NUMBER;
    LuaState["SMALL_NUMBER"] = SMALL_NUMBER;

    UpdateStub();
}

void FLuaScriptManager::InitPIEScripts(TArray<AActor*>& Actors)
{
    SharedEnvironment = sol::environment(LuaState, sol::create, LuaState.globals());
    for (AActor* Actor: Actors)
    {
        // TODO: PIE로 복사됐을 때 Original의 Label을 찾아가야 하나?
        SharedEnvironment[GetData(Actor->GetDefaultActorLabel())] = Actor;
    }
}

void FLuaScriptManager::Reload()
{
    TArray<FString> FilePaths;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(GetData(BasePath)))
    {
        if (entry.path().extension() == ".lua")
        {
            std::filesystem::path absolutePath = std::filesystem::absolute(entry.path());
            FilePaths.Add(absolutePath.generic_string());
        }
    }

    for (const auto& path: FilePaths)
    {
        if (IsFileOutdated(path))
        {
            LoadFile(path);
            for (const auto iter : TObjectRange<ULuaScriptComponent>())
            {
                iter->LoadScriptAndBind();
            }
        }
    }
}

void FLuaScriptManager::ReloadForce()
{
    TArray<FString> FilePaths;
    for (const auto& entry: std::filesystem::recursive_directory_iterator(GetData(BasePath)))
    {
        if (entry.path().extension() == ".lua")
        {
            FilePaths.Add(entry.path().string());
        }
    }

    for (const auto& path: FilePaths)
    {
        if (IsFileOutdated(path))
        {
            LoadFile(path);
            for (const auto iter : TObjectRange<ULuaScriptComponent>())
            {
                iter->InitEnvironment();
                iter->LoadScriptAndBind();
            }
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

    // TArray
    
    
}

void FLuaScriptManager::BindUObject()
{
    sol::usertype<UObject> UObjectTypeTable = LuaState.new_usertype<UObject>("UObject");
    UObjectTypeTable["GetUUID"] = &UObject::GetUUID;
    UObjectTypeTable["GetName"] = &UObject::GetName;
    UObjectTypeTable["GetClass"] = &UObject::GetClass;
    UObjectTypeTable["GetWorld"] = &UObject::GetWorld;
    UObjectTypeTable["IsA"] = static_cast<bool (UObject::*)(const UClass*) const>(&UObject::IsA); // 오버라이드 함수 명시화.

    sol::usertype<UClass> UClassTypeTable = LuaState.new_usertype<UClass>("UClass");
    UClassTypeTable["IsChildOf"] = static_cast<bool (UClass::*)(const UClass*) const>(&UClass::IsChildOf);
    UClassTypeTable["GetSuperClass"] = &UClass::GetSuperClass;

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

void FLuaScriptManager::UpdateStub()
{
    std::filesystem::path basePath(GetData(StubBasePath));
    if (!exists(basePath))
    {
        if (!std::filesystem::create_directory(basePath))
        {
            UE_LOG(ELogLevel::Error, "Failed to create Lua stub directory %s", basePath);
            return;
        };
    }

    if (!is_directory(basePath))
    {
        UE_LOG(ELogLevel::Error, "Can not open Lua stub directory %s", basePath);
        return;
    }
    
    {
        std::filesystem::path filePath = basePath / "UObject.lua";
        std::ofstream file(filePath.string());
        if (!file.is_open())
        {
            UE_LOG(ELogLevel::Error, "Failed to open %s", filePath.c_str());
            return;
        }
        // UObject
        file << "---@class UObject\n";
        file << "local UObject = {}\n";
        file << "\n";
        file << "---@return number\n";
        file << "function UObject:GetUUID() end\n";
        file << "\n";
        file << "---@return string\n";
        file << "function UObject:GetName() end\n";
        file << "\n";
        file << "---@return UClass\n";
        file << "function UObject:GetClass() end\n";
        file << "\n";
        file << "---@param uclass UClass\n";
        file << "---@return bool\n";
        file << "function UObject:IsA(uclass) end\n";
        file << "\n";
        file << "---@return UWorld\n";
        file << "function UObject:GetWorld() end\n";
        file << "\n";
    }
    {
        std::filesystem::path filePath = basePath / "UClass.lua";
        std::ofstream file(filePath.string());
        if ( !file.is_open() ) {
            UE_LOG(ELogLevel::Error, "Failed to open %s", filePath.c_str());
            return;
        }
        // UClass
        file << "---@class UClass\n";
        file << "local UClass = {}\n";
        file << "\n";
        file << "---@return bool\n";
        file << "function UClass:IsChildOf() end\n";
        file << "\n";
        file << "---@return UClass\n";
        file << "function UClass:GetSUperClass() end\n";
        file << "\n";
    }
    for (const auto& [name, type]: UClass::GetClassMap())
    {
        std::filesystem::path filePath = basePath / GetData(name.ToString() + ".lua");
        std::ofstream file(filePath.string());
        if (!file.is_open())
        {
            UE_LOG(ELogLevel::Error, "Failed to open %s", filePath.c_str());
            continue;
        }
        file << type->Annotation.Annotation() << '\n';
        file.close();
    }
    for (const auto& [name, type]: UScriptStruct::GetScriptStructMap())
    {
        std::filesystem::path filePath = basePath / GetData(name.ToString() + ".lua");
        std::ofstream file(filePath.string());
        if (!file.is_open())
        {
            UE_LOG(ELogLevel::Error, "Failed to open %s", filePath.c_str());
            continue;
        }
        file << type->Annotation.Annotation() << '\n';
        file.close();
    }
    {
        std::filesystem::path filePath = basePath / "_common.lua";
        std::ofstream file(filePath.string());
        if (!file.is_open())
        {
            UE_LOG(ELogLevel::Error, "Failed to open %s", filePath.c_str());
            return;
        }

        // obj
        file << "---@type AActor\n";
        file << "_G.obj = {}\n";
        file << "\n";

        // SMALL_NUMBER
        file << "---@type number\n";
        file << "KINDA_SMALL_NUMBER = ";
        file << KINDA_SMALL_NUMBER;
        file << "\n\n";
        file << "---@type number\n";
        file << "SMALL_NUMBER = ";
        file << SMALL_NUMBER;
        file << "\n\n";
        
        // ToString
        file << "---@param obj Object\n";
        file << "function ToString(obj) end\n";
        file << "\n";
        
        // PrintObj
        file << "---@param obj Object\n";
        file << "function PrintObj(obj) end\n";
        file << "\n";
        
        // LogDisplay
        file << "---@param str string\n";
        file << "function LogDisplay(str) end\n";
        file << "\n";
        
        // LogWarning
        file << "---@param str string\n";
        file << "function LogWarning(str) end\n";
        file << "\n";
        
        // LogError
        file << "---@param str string\n";
        file << "function LogError(str) end\n";
        file << "\n";
        
        // SpawnActor
        file << "---@param ActorName string\n";
        file << "function SpawnActor(ActorName) end\n";
        file << "\n";


        // FVector
        file << "---@class FVector\n";
        file << "---@field x number\n";
        file << "---@field y number\n";
        file << "---@field z number\n";
        file << "local FVector = {}\n";
        file << "\n";
        file << "---@param other FVector\n";
        file << "---@return FVector\n";
        file << "function FVector:Dot(other) end\n";
        file << "\n";
        file << "---@param other FVector\n";
        file << "---@return FVector\n";
        file << "function FVector:Cross(other) end\n";
        file << "\n";
        file << "---@return number\n";
        file << "function FVector:Length() end\n";
        file << "---@return number\n";
        file << "function FVector:SquaredLength() end\n";
        file << "\n";
        // TODO: ...

        // FRotator
        file << "---@class FRotator\n";
        file << "---@field Pitch number\n";
        file << "---@field Yaw number\n";
        file << "---@field Roll number\n";
        file << "local FRotator = {}\n";
        file << "\n";

        // FString
        file << "---@class FString\n";
        file << "local FString = {}\n";
        file << "\n";

        file.close();
    }
}

bool FLuaScriptManager::LoadFile(const FString& FileName)
{
    std::string FileNameStr = GetData(FileName);
    std::filesystem::path path(FileNameStr);
    if (path.extension() != ".lua")
    {
        return false;
    }
    
    std::ifstream file(path);
    if (!file.is_open())
    {
        // Cannot open File
        UE_LOG(ELogLevel::Error, "Failed to open %s", FileNameStr.c_str());
        return false;
    }
    if (!file.good())
    {
        // Cannot read File
        UE_LOG(ELogLevel::Error, "Failed to read %s", FileNameStr.c_str());
        return false;
    }
    if (file.eof())
    {
        UE_LOG(ELogLevel::Warning, "File %s is empty", FileNameStr.c_str());
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    auto lastWriteTime = std::filesystem::last_write_time(FileNameStr);

    LuaScripts[FileName] = FLuaScriptInfo(
        FileNameStr,
        source,
        lastWriteTime
    );

    file.close();
    UE_LOG(ELogLevel::Display, "Loaded LuaScript %s", GetData(FileName));

    return true;
}

bool FLuaScriptManager::IsFileOutdated(const FString& FileName)
{
    std::string FileNameStr = GetData(FileName);
    if (!std::filesystem::exists(FileNameStr))
        return false;
    
    auto currentTime = std::filesystem::last_write_time(FileNameStr);
    std::filesystem::file_time_type lastModifiedTime = LuaScripts[FileName].LastModifiedTime;

    // 만약 메인 파일의 저장된 타임스탬프가 없거나 현재와 다르면 변경된 것으로 처리
    if (lastModifiedTime != currentTime)
        return true;
        
    return false;
}

void FLuaScriptManager::PanicHandler(sol::optional<std::string> MaybeMsg)
{
    UE_LOG(ELogLevel::Error, "sol2 Panic State");
    if (MaybeMsg)
    {
        const std::string& msg = MaybeMsg.value();
        UE_LOG(ELogLevel::Error, "%s", msg.c_str());
    }
}

std::string FLuaScriptManager::ToString(const sol::object& obj, int depth, bool showHidden)
{
    if (obj.get_type() == sol::type::nil) {
        return "nil";
    } else if (obj.is<std::string>()) {
        return "\"" + obj.as<std::string>() + "\"";
    } else if (obj.is<int>()) {
        return std::to_string(obj.as<int>());
    } else if (obj.is<double>()) {
        return std::to_string(obj.as<double>());
    } else if (obj.is<bool>()) {
        return obj.as<bool>() ? "true" : "false";
    } else if (obj.get_type() == sol::type::table) {
        std::string result = "{";
        sol::table tbl = obj;
        bool first = true;
        for (auto& kv : tbl) {
            if (!showHidden && kv.first.as<std::string>().starts_with("__"))
                continue;
            if (!first) result += ", ";
            first = false;
            result += ToString(kv.first, depth + 1, showHidden) + " : " + ToString(kv.second, depth + 1, showHidden);
        }
        result += "}";
        return result;
    } else  if (obj.get_type() == sol::type::userdata) {
        sol::table tbl = obj;
        sol::table metatable = tbl[sol::metatable_key];
        std::string type = metatable["__type"]["name"];
        if (obj.is<FVector>())
        {
            FVector vec = obj.as<FVector>();
            std::string result = "[FVector](";
            result += GetData(vec.ToString());
            result += ")";
            return result;
        }
        else if (obj.is<FString>())
        {
            std::string result = "[FString]\"";
            result += GetData(obj.as<FString>());
            result += "\"";
            return result;
        }
        else if (obj.is<FRotator>())
        {
            FRotator rot = obj.as<FRotator>();
            std::string result = "[FRotator](";
            result += GetData(rot.ToString());
            result += ")";
            return result;
        }
        else
        {
            std::string result = "[";
            result += metatable["__type"]["name"];
            result += "]:";
            result += ToString(metatable, depth + 1, false);
            return result;
        }
    } else if (obj.get_type() == sol::type::function) {
        return "[function]";
    } else {
        return "[unknown type]";
    }
}
