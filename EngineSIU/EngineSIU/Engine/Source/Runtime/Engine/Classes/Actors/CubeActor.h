#pragma once
#include "GameFramework/Actor.h"

class UBoxComponent;


class ACubeActor : public AActor
{
    DECLARE_CLASS(ACubeActor, AActor)
public:
    ACubeActor();

    UBoxComponent* GetShapeComponent() const;
protected:
    UPROPERTY
    (UBoxComponent*, BoxComponent, = nullptr)
};

