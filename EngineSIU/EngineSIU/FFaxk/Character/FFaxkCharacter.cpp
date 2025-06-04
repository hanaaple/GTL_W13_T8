#include "FFaxkCharacter.h"

#include <thread>

#include "PhysicsManager.h"
#include "Camera/CameraComponent.h"
#include "Camera/Shakes/SimpleCameraShakePattern.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Contents/Actors/Fish.h"
#include "Engine/Contents/Actors/RotatePatrolPlatform.h"
#include "Engine/Contents/Actors/SpringTrap.h"
#include "Engine/Contents/Actors/TranslateSpringTrap.h"
#include "FFaxk/FFaxkGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "LuaScripts/LuaScriptComponent.h"
#include "World/World.h"

AFFaxkCharacter::AFFaxkCharacter()
{
    LuaComponent = AddComponent<ULuaScriptComponent>();
    std::filesystem::path GamePath = std::filesystem::absolute("LuaScripts/inputtest.lua");
    
    LuaComponent->SetScriptPath(GamePath.generic_string());

    USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
    SkeletalMeshComponent->SetSkeletalMeshAsset(UAssetManager::Get().GetSkeletalMesh("Contents/Character/Paladin_J_Nordstrom"));

    FString AssetKey = "Contents/PhysicsAsset/UPhysicsAsset_212";
    UObject* AssetObject = UAssetManager::Get().GetAsset(EAssetType::PhysicsAsset, FName(AssetKey));
    if (UPhysicsAsset* PhysicsAsset = Cast<UPhysicsAsset>(AssetObject))
    {
        if (PhysicsAsset->SetPreviewMesh(SkeletalMeshComponent->GetSkeletalMeshAsset()))
        {
            SkeletalMeshComponent->GetSkeletalMeshAsset()->SetPhysicsAsset(PhysicsAsset);
        }
    }
    
    
    SpringArm = AddComponent<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArm->bEnableCameraLag = false;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->TargetOffset = FVector(-15.0f, 0.0f, 150.0f);
    SpringArm->TargetArmLength = 200.0f;
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = AddComponent<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->FarClip = 25000.0f;
    CameraComponent->SetupAttachment(SpringArm);
}

void AFFaxkCharacter::BeginPlay()
{
    ACharacter::BeginPlay();
    
    OnActorBeginOverlap.AddUObject(this, &AFFaxkCharacter::OnBeginOverlap);
    OnActorEndOverlap.AddUObject(this, &AFFaxkCharacter::OnEndOverlap);
}

void AFFaxkCharacter::Tick(float DeltaTime)
{
    ACharacter::Tick(DeltaTime);

    if (GetActorLocation().Z < 0.0f || GetActorLocation().Z > 2500.0f)
    {
        if (GEngine->ActiveWorld)
        {
            if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
            {
                if (GameMode->GetGameState() == EGameState::Playing)
                {
                    Die();
                }
            }
        }
    }
}

void AFFaxkCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    float Power = 0;
    if (OtherActor->IsA<AFish>())
    {
        // TODO Temp Code 사실상 상수처럼 쓰고 있음
        Power = Cast<AFish>(OtherActor)->Power;
    }
    else if (OtherActor->IsA<ASpringTrap>())
    {
        Power = Cast<ASpringTrap>(OtherActor)->ShootSpeed;
    }
    else if (OtherActor->IsA<ARotatePatrolPlatform>())
    {
        Power = Cast<ARotatePatrolPlatform>(OtherActor)->AngularSpeed;
    }
    else if (OtherActor->IsA<ATranslateSpringTrap>())
    {
        Power = Cast<ATranslateSpringTrap>(OtherActor)->ShootSpeed;
    }

    if (FMath::Abs(Power) > 0.1)
    {
        for (UPrimitiveComponent* PrimitiveComponent : OverlappedActor->GetComponentsByClass<UPrimitiveComponent>())
        {
            if (PrimitiveComponent->BodyInstance && PrimitiveComponent->BodyInstance->BIGameObject)
            {
                // PrimitiveComponent->DestroyPhysXGameObject();
                PrimitiveComponent->BodyInstance->BIGameObject->DynamicRigidBody->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
            }
        }

        for (USkeletalMeshComponent* SkeletalMeshComponent : OverlappedActor->GetComponentsByClass<USkeletalMeshComponent>())
        {
            SkeletalMeshComponent->RigidBodyType = ERigidBodyType::DYNAMIC;
            SkeletalMeshComponent->bOverrideTransform = true;
            SkeletalMeshComponent->bApplyGravity = true;

            SkeletalMeshComponent->Stop();
            SkeletalMeshComponent->SetSimulate(false);
            SkeletalMeshComponent->SetSimulate(true);
            for (FBodyInstance* Body : SkeletalMeshComponent->GetBodies())
            {
                FVector Direction = (OverlappedActor->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
                Power = 2000;
                FVector Force = Direction * Power * 1000;
                Body->AddBodyForce(Force);
            }
            
            Die();

            std::thread th {[this]()
            {
                extern float GDeltaScale;

                extern float GTargetDeltaScale;
                    
                GTargetDeltaScale = 0.2;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                GTargetDeltaScale = 1;
            }};

            th.detach();
        }
    }
}

void AFFaxkCharacter::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    int a = 0;
}

void AFFaxkCharacter::Die()
{
    if (GEngine->ActiveWorld)
    {
        if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
        {
            if (GameMode->GetGameState() == EGameState::Playing)
            {
                GameMode->SetGameState(EGameState::Died);
                if (APlayerCameraManager* PCM = GameMode->GetPlayerController()->GetCameraManager())
                {
                    PCM->StartCameraFade(0.0f, 1.0f, 3.0f, FLinearColor::Black, true);
                    PCM->OnFadeCompleteEvent.AddWeakLambda(this, [&]()
                    {
                        if (AFFaxkGameMode* GameMode = GEngine->ActiveWorld->GetGameMode<AFFaxkGameMode>())
                        {
                            GameMode->SetGameState(EGameState::End);
                        }
                    });
                }
            }
        }
    }
}
