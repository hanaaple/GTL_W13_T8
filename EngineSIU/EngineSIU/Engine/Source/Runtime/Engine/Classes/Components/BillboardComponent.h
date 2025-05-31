#pragma once

#define _TCHAR_DEFINED
#include <wrl.h>
#include "PrimitiveComponent.h"


class UBillboardComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UBillboardComponent, UPrimitiveComponent)

public:
    UBillboardComponent();
    virtual ~UBillboardComponent() override = default;
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;
    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(const FVector& InRayOrigin, const FVector& InRayDirection, float& OutHitDistance) const override;

    virtual void SetTexture(const FWString& InFilePath);
    void SetUUIDParent(USceneComponent* InUUIDParent);
    FMatrix CreateBillboardMatrix() const;
    FString GetTexturePath() const { return TexturePath; }

    float FinalIndexU = 0.0f;
    float FinalIndexV = 0.0f;
    std::shared_ptr<FTexture> Texture;

    bool bIsEditorBillboard = false;

protected:
    UPROPERTY(USceneComponent*, UUIDParent, = nullptr)
    UPROPERTY(FString, TexturePath, = TEXT("default"))

    // NDC 픽킹을 위한 내부 함수 : quadVertices는 월드 공간 정점 배열
    bool CheckPickingOnNDC(const TArray<FVector>& QuadVertices, float& HitDistance) const;

public:
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
};
