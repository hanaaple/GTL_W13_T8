#pragma once
#include "GameFramework/PlayerController.h"


class AFFaxkController : public APlayerController
{
    DECLARE_CLASS(AFFaxkController, APlayerController)

public:
    AFFaxkController() = default;
    virtual ~AFFaxkController() override = default;

protected:
    void RequestRespawn();
};
