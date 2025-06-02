#include "FFaxkGameMode.h"
#include "FFaxkController.h"
#include "Character/FFaxkCharacter.h"


AFFuaxkGameMode::AFFuaxkGameMode()
{
    DefaultPawnClass = AFFaxkCharacter::StaticClass();
    PlayerControllerClass = AFFaxkController::StaticClass();
}

void AFFuaxkGameMode::InitGame()
{
    Super::InitGame();

    GameState = EGameState::MainWidget;
}

void AFFuaxkGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AFFuaxkGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
