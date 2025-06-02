#pragma once

#include "Classes/Engine/StaticMeshActor.h"

class ASphere : public AStaticMeshActor
{
    DECLARE_CLASS(ASphere, AStaticMeshActor)

public:
    ASphere();

    virtual void Tick(float DeltaTime) override;
};

