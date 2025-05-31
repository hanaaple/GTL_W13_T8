#pragma once
#include "Runtime/CoreUObject/UObject/ObjectMacros.h"
#include "Components/ActorComponent.h"
#include <sol/sol.hpp>
#include <filesystem>

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLocationTenUp, const FVector);

class ULuaScriptComponent : public UActorComponent
{
    DECLARE_CLASS(ULuaScriptComponent, UActorComponent)

public:
    ULuaScriptComponent();
    ~ULuaScriptComponent();

    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& Properties) override;

    FString BasePath = FString(L"LuaScripts");

    TMap<FString, sol::object> ExposedProperties;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime) override;
    virtual void InitializeComponent() override;

    // Lua 함수 호출 메서드
    template<typename... Arguments> void CallLuaFunction(const FString& FunctionName, Arguments... args);
    
    FString GetScriptPath() const { return ScriptPath; }
    void SetScriptPath(const FString& InScriptPath);

    FString GetDisplayName() const { return DisplayName; }
    void SetDisplayName(const FString& InDisplayName) { DisplayName = InDisplayName; }

    void OnPressSpacebar()
    {
        UE_LOG(ELogLevel::Error, "Deligate Space Press");
    }

    FOnLocationTenUp FOnLocationTenUp;

    void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
    void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
    
private:
    // Lua 환경 초기화
    void InitializeLuaState();

    // Lua-Engine 바인딩
    void BindEngineAPI();

    UPROPERTY(FString, ScriptPath, = TEXT("None"))
    UPROPERTY(FString, DisplayName, = TEXT("None"))

    TArray<FDelegateHandle> DelegateHandles;
    
    sol::state LuaState;
    bool bScriptValid = false;

    std::filesystem::file_time_type LastWriteTime;
    bool CheckFileModified();
    void ReloadScript();
};

template <typename ... Arguments>
void ULuaScriptComponent::CallLuaFunction(const FString& FunctionName, Arguments... args)
{
    if (bScriptValid && LuaState[*FunctionName].valid())
    {
        LuaState[*FunctionName](args...);
    }
}
