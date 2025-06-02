#pragma once
#include "GameFramework/GameModeBase.h"


enum class EGameState : uint8
{
    None,
    MainWidget, // 메인 화면 UI
    Playing,    // 게임 플레이중
    Paused,     // ESC로 일시정지
};


class AFFuaxkGameMode : public AGameModeBase
{
    DECLARE_CLASS(AFFuaxkGameMode, AGameModeBase)

public:
    AFFuaxkGameMode();
    virtual ~AFFuaxkGameMode() override = default;

    virtual void InitGame() override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    [[nodiscard]] EGameState GetGameState() const { return GameState; }
    void SetGameState(EGameState InGameState) { GameState = InGameState; }

private:
    // 현재 게임 상태
    EGameState GameState = EGameState::None;

private:
    // 리스폰시 사용될 마지막 플레이어 위치
    FVector LastPlayerLocation;
};
