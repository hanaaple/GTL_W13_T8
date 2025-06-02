#include "World.h"


#include "CollisionManager.h"
#include "PhysicsManager.h"
#include "Actors/Player.h"
#include "BaseGizmos/TransformGizmo.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Engine/FObjLoader.h"
#include "Engine/Engine.h"
#include "UnrealEd/SceneManager.h"
#include "GameFramework/GameMode.h"
#include "Classes/Components/TextComponent.h"

class UEditorEngine;

UWorld::~UWorld()
{
    if (GEngine->PhysicsManager->GetScene(this))
    {
        GEngine->PhysicsManager->RemoveScene(this);
    }
}

UWorld* UWorld::CreateWorld(UObject* InOuter, const EWorldType InWorldType, const FString& InWorldName)
{
    UWorld* NewWorld = FObjectFactory::ConstructObject<UWorld>(InOuter);
    NewWorld->WorldName = InWorldName;
    NewWorld->WorldType = InWorldType;
    NewWorld->InitializeNewWorld();

    
    return NewWorld;
}

void UWorld::InitializeNewWorld()
{
    ActiveLevel = FObjectFactory::ConstructObject<ULevel>(this);
    ActiveLevel->InitLevel(this);

    CollisionManager = new FCollisionManager();
}

// UObject* UWorld::Duplicate(UObject* InOuter)
// {
//     // TODO: UWorld의 Duplicate는 역할 분리후 만드는것이 좋을듯
//     UWorld* NewWorld = Cast<UWorld>(Super::Duplicate(InOuter));
//     NewWorld->ActiveLevel = Cast<ULevel>(ActiveLevel->Duplicate(NewWorld));
//     NewWorld->ActiveLevel->InitLevel(NewWorld);
//     NewWorld->GameModeClass = GameModeClass;
//     
//     NewWorld->CollisionManager = new FCollisionManager();
//     
//     return NewWorld;
// }

void UWorld::Tick(float DeltaTime)
{
    TimeSeconds += DeltaTime;
    
    // SpawnActor()에 의해 Actor가 생성된 경우, 여기서 BeginPlay 호출
    if (WorldType != EWorldType::Editor)
    {
        for (AActor* Actor : PendingBeginPlayActors)
        {
            Actor->BeginPlay();
        }
        PendingBeginPlayActors.Empty();
    }
}

void UWorld::BeginPlay()
{
    for (AActor* Actor : ActiveLevel->Actors)
    {
        if (Actor->GetWorld() == this)
        {
            Actor->BeginPlay();
        }
    }
}

void UWorld::Release()
{
    if (ActiveLevel)
    {
        ActiveLevel->Release();
        GUObjectArray.MarkRemoveObject(ActiveLevel);
        ActiveLevel = nullptr;
    }

    if (CollisionManager)
    {
        delete CollisionManager;
        CollisionManager = nullptr;
    }
    
    //GUObjectArray.ProcessPendingDestroyObjects();
}

AActor* UWorld::SpawnActor(UClass* InClass, FName InActorName)
{
    if (!InClass)
    {
        UE_LOG(ELogLevel::Error, TEXT("SpawnActor failed: ActorClass is null."));
        return nullptr;
    }

    
    // TODO: SpawnParams에서 이름 가져오거나, 필요시 여기서 자동 생성
    // if (SpawnParams.Name != NAME_None) ActorName = SpawnParams.Name;
    
    if (InClass->IsChildOf<AActor>())
    {
        AActor* NewActor = Cast<AActor>(FObjectFactory::ConstructObject(InClass, this, InActorName));
        // TODO: 일단 AddComponent에서 Component마다 초기화
        // 추후에 RegisterComponent() 만들어지면 주석 해제
        // Actor->InitializeComponents();
        ActiveLevel->Actors.Add(NewActor);
        PendingBeginPlayActors.Add(NewActor);

        NewActor->PostSpawnInitialize();

        if (NewActor->GetRootComponent() == nullptr)
        {
            NewActor->SetRootComponent(NewActor->AddComponent<USceneComponent>());
        }
        
        return NewActor;
    }
    
    UE_LOG(ELogLevel::Error, TEXT("SpawnActor failed: Class '%s' is not derived from AActor."), *InClass->GetName());
    return nullptr;
}

bool UWorld::DestroyActor(AActor* ThisActor)
{
    if (ThisActor->GetWorld() == nullptr)
    {
        return false;
    }

    if (ThisActor->IsActorBeingDestroyed())
    {
        return true;
    }
    
    // UEditorEngine* Engine = Cast<UEditorEngine>(GEngine);
    //
    // Engine->DeselectActor(ThisActor);

    // 액터의 Destroyed 호출
    ThisActor->Destroyed();

    if (ThisActor->GetOwner())
    {
        ThisActor->SetOwner(nullptr);
    }

    TSet<UActorComponent*> Components = ThisActor->GetComponents();
    for (UActorComponent* Component : Components)
    {
        Component->DestroyComponent();
    }

    // World에서 제거
    ActiveLevel->Actors.Remove(ThisActor);

    // 제거 대기열에 추가
    GUObjectArray.MarkRemoveObject(ThisActor);
    return true;
}

UWorld* UWorld::GetWorld() const
{
    return const_cast<UWorld*>(this);
}

APawn* UWorld::GetMainPlayer() const
{
    if (MainPlayer)
    {
        return MainPlayer;
    }
    return nullptr;
}

APlayerController* UWorld::GetPlayerController() const
{
    if (PlayerController)
    {
        return PlayerController;
    }

    //메인플레이어컨트롤러 설정안하면 있는거중 한개
    for (const auto Iter: TObjectRange<APlayerController>())
    {
        if (Iter->GetWorld() == GEngine->ActiveWorld)
        {
            return Iter;
        }
    }

    return nullptr;
}

void UWorld::SetGameModeClass(const TSubclassOf<AGameModeBase>& InGameModeClass)
{
    if (InGameModeClass == nullptr)
    {
        UE_LOG(ELogLevel::Error, TEXT("SetGameModeClass failed: GameModeClass is null."));
        return;
    }
    GameModeClass = InGameModeClass;
}

void UWorld::InitGameMode()
{
    if (
        WorldType == EWorldType::Editor           // 일반적인 에디터 환경
        || WorldType == EWorldType::EditorPreview // 머티리얼 에디터나 애님 블루프린트 같은 프리뷰 뷰포트
    )
    {
        UE_LOG(ELogLevel::Display, TEXT("Skipping GameMode spawn for Editor/Preview world."));
        return;
    }

    if (GameModeInstance)
    {
        UE_LOG(ELogLevel::Warning, TEXT("GameMode already exists in this world. Skipping spawn."));
        return;
    }

    UClass* SubClass = GameModeClass.Get();
    if (!SubClass)
    {
        SubClass = AGameModeBase::StaticClass();
    }

    AGameModeBase* SpawnGameMode = Cast<AGameModeBase>(SpawnActor(SubClass, "__World_GameMode__"));
    SpawnGameMode->InitGame();
    GameModeInstance = SpawnGameMode;
}

void UWorld::DestroyGameMode()
{
    if (GameModeInstance)
    {
        DestroyActor(GameModeInstance);
        GameModeInstance = nullptr;
    }
}

void UWorld::CheckOverlap(const UPrimitiveComponent* Component, TArray<FOverlapResult>& OutOverlaps) const
{
    if (CollisionManager)
    {
        CollisionManager->CheckOverlap(this, Component, OutOverlaps);
    }
}

void UWorld::DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator)
{
    Super::DuplicateSubObjects(Source, InOuter, Duplicator);
}

void UWorld::PostDuplicate()
{
    UObject::PostDuplicate();
    ActiveLevel->InitLevel(this);
    CollisionManager = new FCollisionManager();
}
