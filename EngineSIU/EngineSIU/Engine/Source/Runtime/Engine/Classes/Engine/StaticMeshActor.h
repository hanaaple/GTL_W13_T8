#pragma once
#include "GameFramework/Actor.h"

class UStaticMeshComponent;


class AStaticMeshActor : public AActor
{
    DECLARE_CLASS(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();

    UStaticMeshComponent* GetStaticMeshComponent() const;
    //virtual UObject* Duplicate(UObject* InOuter) override;

protected:
    UPROPERTY
    (UStaticMeshComponent*, StaticMeshComponent, = nullptr);
};
