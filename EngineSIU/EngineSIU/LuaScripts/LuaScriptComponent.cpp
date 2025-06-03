#include "LuaScriptComponent.h"
#include "LuaBindingHelpers.h"
#include "LuaScriptFileUtils.h"
#include "World/World.h"
#include "Engine/EditorEngine.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

ULuaScriptComponent::ULuaScriptComponent()
{
}

void ULuaScriptComponent::GetProperties(TMap<FString, FString>& OutProperties) const
{
    Super::GetProperties(OutProperties);
    OutProperties.Add(TEXT("ScriptPath"), *ScriptPath);
    OutProperties.Add(TEXT("DisplayName"), *DisplayName);
}

void ULuaScriptComponent::SetProperties(const TMap<FString, FString>& Properties)
{
    const FString* TempStr = nullptr;

    TempStr = Properties.Find(TEXT("ScriptPath"));
    if (TempStr)
    {
        this->ScriptPath = *TempStr;
    }
    TempStr = Properties.Find(TEXT("DisplayName"));
    if (TempStr)
    {
        this->DisplayName = *TempStr;
    }
}

void ULuaScriptComponent::BeginPlay()
{
    Super::BeginPlay();

    LoadScriptAndBind();
    GetOwner()->OnActorBeginOverlap.AddUObject(this, &ULuaScriptComponent::OnBeginOverlap);
    GetOwner()->OnActorEndOverlap.AddUObject(this, &ULuaScriptComponent::OnEndOverlap);


    DelegateHandles.Empty();
    
    CallLuaFunction("BeginPlay");
}

void ULuaScriptComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
    Super::EndPlay(EndPlayReason);

    CallLuaFunction("EndPlay");
}

/* ActorComponent가 Actor와 World에 등록이 되었다는 전제하에 호출됩니다
 * So That we can use GetOwner() and GetWorld() safely
 */
void ULuaScriptComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (ScriptPath.IsEmpty()) {
        bool bSuccess = LuaScriptFileUtils::MakeScriptPathAndDisplayName(
            L"template.lua",
            GetOwner()->GetWorld()->GetName().ToWideString(),
            GetOwner()->GetName().ToWideString(),
            ScriptPath,
            DisplayName
        );
        if (!bSuccess) {
            UE_LOG(ELogLevel::Error, TEXT("Failed to create script from template"));
            return;
        }
    }
}

void ULuaScriptComponent::SetScriptPath(const FString& InScriptPath)
{
    ScriptPath = InScriptPath;
    bScriptValid = false;
}

void ULuaScriptComponent::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    CallLuaFunction("BeginOverlap", OtherActor);
}

void ULuaScriptComponent::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    CallLuaFunction("EndOverlap", OtherActor);
}

void ULuaScriptComponent::LoadScriptAndBind()
{
    if (!FEngineLoop::ScriptSys.LuaScripts.Contains(ScriptPath))
    {
        UE_LOG(ELogLevel::Error, "Can not find %s", GetData(ScriptPath));
        return;
    }
    
    if (!CompEnvironment.valid())
    {
        InitEnvironment();
    }
    CompEnvironment["obj"] = GetOwner();

    const FLuaScriptInfo& scriptInfo = FEngineLoop::ScriptSys.LuaScripts[ScriptPath];
    
    // compile script
    sol::load_result loadResult = FEngineLoop::ScriptSys.GetState().load_buffer(scriptInfo.Source.c_str(), scriptInfo.Source.length());

    if (!loadResult.valid())
    {
        // compile error
        sol::error err = loadResult;
        UE_LOG(ELogLevel::Error, "Failed to compile %s@%s", GetData(ScriptPath), err.what());
        bScriptValid = false;
        return;
    }
    
    sol::function script = loadResult;
    if (!script.valid())
    {
        sol::error err = loadResult;
        UE_LOG(ELogLevel::Error, "Failed to compile %s@%s", GetData(ScriptPath), err.what());
        bScriptValid = false;
        return;
    }

    // execute script
    sol::set_environment(CompEnvironment, script);
    sol::protected_function_result res = script();
    if (!res.valid())
    {
        // execute error
        sol::error err = res;
        UE_LOG(ELogLevel::Error, "Failed to execute %s@%s", GetData(ScriptPath), err.what());
        bScriptValid = false;
        return;
    }
    bScriptValid = true;
}

void ULuaScriptComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
    CallLuaFunction("Tick", DeltaTime);
}

void ULuaScriptComponent::InitEnvironment()
{
    sol::state& lua = FEngineLoop::ScriptSys.GetState();
    CompEnvironment = sol::environment(lua, sol::create, FEngineLoop::ScriptSys.GetSharedEnvironment());
}
