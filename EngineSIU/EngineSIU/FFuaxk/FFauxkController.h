#pragma once
#include "GameFramework/PlayerController.h"


class AFFauxkController : public APlayerController
{
    DECLARE_CLASS(AFFauxkController, APlayerController)

public:
    AFFauxkController() = default;
    virtual ~AFFauxkController() override = default;
};
