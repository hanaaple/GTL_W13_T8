#pragma once
#include "StaticMeshComponent.h"

class UCubeComp : public UStaticMeshComponent
{
    DECLARE_CLASS(UCubeComp, UStaticMeshComponent)

public:
    UCubeComp();

    virtual void InitializeComponent() override;
};
