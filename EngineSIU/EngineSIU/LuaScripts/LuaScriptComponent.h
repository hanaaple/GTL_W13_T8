#pragma once
#include "Runtime/CoreUObject/UObject/ObjectMacros.h"
#include "Components/ActorComponent.h"
#include <sol/sol.hpp>
#include <filesystem>

#include "World/World.h"

class ULuaScriptComponent : public UActorComponent
{
    DECLARE_CLASS(ULuaScriptComponent, UActorComponent)

public:
    ULuaScriptComponent();

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& Properties) override;

    FString BasePath = FString(L"LuaScripts");

    TMap<FString, sol::object> ExposedProperties;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime) override;
    void InitEnvironment();

    virtual void InitializeComponent() override;
    void LoadScriptAndBind();

    // Lua 함수 호출 메서드
    template<typename... Arguments> void CallLuaFunction(const FString& FunctionName, Arguments... args);
    
    FString GetScriptPath() const { return ScriptPath; }
    void SetScriptPath(const FString& InScriptPath);

    FString GetDisplayName() const { return DisplayName; }
    void SetDisplayName(const FString& InDisplayName) { DisplayName = InDisplayName; }

    void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
    void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
    sol::environment CompEnvironment;

    UPROPERTY(FString, ScriptPath, = TEXT("None"))
    UPROPERTY(FString, DisplayName, = TEXT("None"))

    TArray<FDelegateHandle> DelegateHandles;
    
    bool bScriptValid = false;

};

template <typename ... Arguments>
void ULuaScriptComponent::CallLuaFunction(const FString& FunctionName, Arguments... args)
{
    // TODO: PIE에서만 tick이 돌아가게 해야 되지 않을까...
    if (this->GetWorld()->WorldType != EWorldType::PIE)
    {
        return;
    }
    if (!bScriptValid)
    {
        UE_LOG(ELogLevel::Error, "%s: Invalid Script", GetData(ScriptPath));
        return;
    }
    if (!CompEnvironment.valid())
    {
        UE_LOG(ELogLevel::Error, "%s: Invalid Environment", GetData(ScriptPath));
        return;
    }
    if (!CompEnvironment[GetData(FunctionName)].valid())
    {
        UE_LOG(ELogLevel::Error, "%s: Invalid Function: %s", GetData(ScriptPath), GetData(FunctionName));
        return;
    }
    sol::protected_function_result res = CompEnvironment[GetData(FunctionName)](args...);
    if (!res.valid())
    {
        sol::error err = res;
        UE_LOG(ELogLevel::Error, "%s: Error while execute Function: %s: %s", GetData(ScriptPath), GetData(FunctionName), err.what());
    }
}
