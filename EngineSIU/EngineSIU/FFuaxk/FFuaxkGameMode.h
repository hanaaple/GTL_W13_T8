#pragma once
#include "GameFramework/GameModeBase.h"


class AFFuaxkGameMode : public AGameModeBase
{
    DECLARE_CLASS(AFFuaxkGameMode, AGameModeBase)

public:
    AFFuaxkGameMode() = default;
    virtual ~AFFuaxkGameMode() override = default;

    virtual void InitGame() override;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
