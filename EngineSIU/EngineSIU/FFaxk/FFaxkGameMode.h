#pragma once
#include "GameFramework/GameModeBase.h"


enum class EGameState : uint8
{
    None,
    MainWidget, // 메인 화면 UI
    Playing,    // 게임 플레이중
    Paused,     // ESC로 일시정지
    Died,       // 플레이어가 사망했을 때
};


class AFFaxkGameMode : public AGameModeBase
{
    DECLARE_CLASS(AFFaxkGameMode, AGameModeBase)

public:
    AFFaxkGameMode();
    virtual ~AFFaxkGameMode() override = default;

    virtual void InitGame() override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    void RequestPlayerRespawn(APlayerController* PlayerController);

    [[nodiscard]] EGameState GetGameState() const { return GameState; }
    void SetGameState(EGameState InGameState) { GameState = InGameState; }

protected:
    void RespawnPlayer(APlayerController* PlayerControllerToRespawn);

private:
    // 현재 게임 상태
    EGameState GameState = EGameState::None;
};
