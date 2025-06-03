#include "FFaxkGameMode.h"
#include "FFaxkController.h"
#include "Character/FFaxkCharacter.h"
#include "World/World.h"


AFFaxkGameMode::AFFaxkGameMode()
{
    DefaultPawnClass = AFFaxkCharacter::StaticClass();
    PlayerControllerClass = AFFaxkController::StaticClass();
}

void AFFaxkGameMode::InitGame()
{
    Super::InitGame();

    GameState = EGameState::MainWidget;
}

void AFFaxkGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AFFaxkGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AFFaxkGameMode::RequestPlayerRespawn(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        // TODO: 리스폰 규칙 설정
        RespawnPlayer(PlayerController);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AFFaxkGameMode::RespawnPlayer(APlayerController* PlayerControllerToRespawn)
{
    if (!PlayerControllerToRespawn || !DefaultPawnClass) // DefaultPawnClass가 설정되어 있는지 확인
    {
        UE_LOG(ELogLevel::Error, TEXT("Cannot respawn: PlayerController or DefaultPawnClass is null."));
        return;
    }

    // 현재 빙의된 폰이 있다면 빙의 해제
    if (PlayerControllerToRespawn->GetPawn())
    {
        PlayerControllerToRespawn->UnPossess();
    }

    // 새로운 빙의
    if (AFFaxkCharacter* NewPawn = GetWorld()->SpawnActor<AFFaxkCharacter>(DefaultPawnClass))
    {
        const FVector PlayerStartLocation = FindPlayerStart()->GetActorLocation();
        NewPawn->SetActorLocation(PlayerStartLocation);
        PlayerControllerToRespawn->Possess(NewPawn);
    }
}
