#pragma once
#include "GameFramework/Character.h"

class ULuaScriptComponent;
class UCameraComponent;
class USpringArmComponent;


class AFFaxkCharacter : public ACharacter
{
    DECLARE_CLASS(AFFaxkCharacter, ACharacter)

public:
    AFFaxkCharacter();
    virtual ~AFFaxkCharacter() override = default;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void Die();

private:
    UPROPERTY(
        VisibleAnywhere, { .Category = "Camera" },
        USpringArmComponent*, SpringArm, ;
    )

    UPROPERTY(
        VisibleAnywhere, { .Category = "Camera" },
        UCameraComponent*, CameraComponent, ;
    )

    UPROPERTY(
        ULuaScriptComponent*, LuaComponent, ;
    )
};
