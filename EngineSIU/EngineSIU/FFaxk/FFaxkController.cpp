#include "FFaxkController.h"
#include "FFaxkGameMode.h"
#include "World/World.h"


void AFFaxkController::BeginPlay()
{
    Super::BeginPlay();

}

void AFFaxkController::RequestRespawn()
{
    AFFaxkGameMode* GameMode = GetWorld()->GetGameMode<AFFaxkGameMode>();
    assert(GameMode && "GameMode is null! or not AFFaxkGameMode");

    GameMode->RequestPlayerRespawn(this);
}
