#pragma once
#include "Define.h"
#include "Container/Set.h"
#include "UObject/ObjectFactory.h"
#include "UObject/ObjectMacros.h"
#include "WorldType.h"
#include "Level.h"
#include "Actors/Player.h"
#include "GameFramework/PlayerController.h"
#include "Engine/EventManager.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/UObjectIterator.h"

class UPrimitiveComponent;
struct FOverlapResult;
class UCameraComponent;
class FObjectFactory;
class AActor;
class UObject;
class USceneComponent;
class FCollisionManager;
class AGameMode;
class UTextComponent;

class UWorld : public UObject
{
    DECLARE_CLASS(UWorld, UObject)

public:
    UWorld() = default;
    virtual ~UWorld() = default;

    static UWorld* CreateWorld(UObject* InOuter, const EWorldType InWorldType, const FString& InWorldName = "DefaultWorld");

    void InitializeNewWorld();
    //virtual UObject* Duplicate(UObject* InOuter) override;

    virtual void Tick(float DeltaTime);
    void BeginPlay();

    void Release();

    /**
     * World에 Actor를 Spawn합니다.
     * @param InClass Spawn할 Actor 정보
     * @return Spawn된 Actor
     */
    AActor* SpawnActor(UClass* InClass, FName InActorName = NAME_None);

    /** 
     * World에 Actor를 Spawn합니다.
     * @tparam T AActor를 상속받은 클래스
     * @return Spawn된 Actor의 포인터
     */
    template <typename T>
        requires std::derived_from<T, AActor>
    T* SpawnActor();

    template <typename T>
    T* SpawnActor(UClass* InClass, FName InActorName = NAME_None)
    {
        return Cast<T>(SpawnActor(InClass, InActorName));
    }

    /** World에 존재하는 Actor를 제거합니다. */
    bool DestroyActor(AActor* ThisActor);

    virtual UWorld* GetWorld() const override;
    UFUNCTION_CONST(ULevel*, GetActiveLevel);

    template <typename T>
        requires std::derived_from<T, AActor>
    T* DuplicateActor(T* InActor);

    EWorldType WorldType = EWorldType::None;
    
    FEventManager EventManager;

    UFUNCTION(void, SetPlayerController, APlayerController* InPlayerController)
    UFUNCTION_CONST(APlayerController*, GetPlayerController);

    //~ GameMode 관련
    AGameModeBase* GetGameMode() const { return GameModeInstance; }

    template <typename T>
        requires std::derived_from<T, AGameModeBase>
    T* GetGameMode() const
    {
        return Cast<T>(GameModeInstance);
    }

    void SetGameModeClass(const TSubclassOf<AGameModeBase>& InGameModeClass);
    UClass* GetGameModeClass() const { return GameModeClass; }

    void InitGameMode();
    void DestroyGameMode();
    // ~GameMode 관련
    
    void CheckOverlap(const UPrimitiveComponent* Component, TArray<FOverlapResult>& OutOverlaps) const;
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
    virtual void PostDuplicate() override;

    double TimeSeconds;

protected:
    /** Game이나 PIE 모드일 때 Spawn되는 GameMode */
    UPROPERTY(
        EditAnywhere,
        TSubclassOf<AGameModeBase>, GameModeClass, ;
    )

    FString WorldName = "DefaultWorld";
    
private:
    AGameModeBase* GameModeInstance = nullptr;

    UPROPERTY(ULevel*, ActiveLevel, = nullptr)

    /** Actor가 Spawn되었고, 아직 BeginPlay가 호출되지 않은 Actor들 */
    TArray<AActor*> PendingBeginPlayActors;

    // TODO: 싱글 플레이어면 상관 없지만, 로컬 멀티 플레이어인 경우를 위해 배열로 관리하는 방법을 고려하기.
    APlayerController* PlayerController = nullptr;

    UTextComponent* MainTextComponent = nullptr;

    FCollisionManager* CollisionManager = nullptr;
};


template <typename T>
    requires std::derived_from<T, AActor>
T* UWorld::SpawnActor()
{
    return Cast<T>(SpawnActor(T::StaticClass()));
}

template <typename T> requires std::derived_from<T, AActor>
T* UWorld::DuplicateActor(T* InActor)
{
    if (ULevel* ActiveLevel = GetActiveLevel())
    {
        T* NewActor = static_cast<T*>(InActor->Duplicate(this));
        ActiveLevel->Actors.Add(NewActor);
        PendingBeginPlayActors.Add(NewActor);
        return NewActor;
    }
    return nullptr;
}


