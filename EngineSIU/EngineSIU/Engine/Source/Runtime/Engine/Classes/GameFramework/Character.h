#pragma once
#include "Pawn.h"

class UCapsuleComponent;
class USkeletalMeshComponent;


class ACharacter : public APawn
{
    DECLARE_CLASS(ACharacter, APawn)

public:
    ACharacter();
    virtual ~ACharacter() override = default;

public:
    UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
    USkeletalMeshComponent* GetMesh() const { return MeshComponent; }

private:
    UPROPERTY(
        VisibleAnywhere,
        UCapsuleComponent*, CapsuleComponent, ;
    )

    UPROPERTY(
        VisibleAnywhere,
        USkeletalMeshComponent*, MeshComponent, ;
    )
};
