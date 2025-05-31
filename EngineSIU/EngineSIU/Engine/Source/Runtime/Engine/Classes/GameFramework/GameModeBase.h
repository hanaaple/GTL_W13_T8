#pragma once
#include "PlayerController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Template/SubclassOf.h"


class AGameModeBase : public AActor
{
    DECLARE_CLASS(AGameModeBase, AActor)

public:
    AGameModeBase();

    /** GameMode에 설정된 기본 클래스를 Spawn합니다. */
    virtual void InitGame();

    /** World에 있는 PlayerStart Actor를 가져옵니다. */
    AActor* FindPlayerStart(const FString& IncomingName = TEXT("")) const;

    /** GameMode에서 관리하는 PlayerController 인스턴스를 반환합니다. */
    APlayerController* GetPlayerController() const { return PlayerControllerInstance; }

public:
    UPROPERTY(
        EditAnywhere | LuaReadOnly, { .Category = "Classes" },
        TSubclassOf<APlayerController>, PlayerControllerClass, ;
    )

    UPROPERTY(
        EditAnywhere | LuaReadOnly, { .Category = "Classes" },
        TSubclassOf<APawn>, DefaultPawnClass, ;
    )

private:
    mutable AActor* OriginActor = nullptr;

    // TODO: 나중에 ULocalPlayer 만들면 옮기기 
    APlayerController* PlayerControllerInstance = nullptr;
};
