#pragma once

#define _TCHAR_DEFINED
#include <wrl.h>

#include "BillboardComponent.h"

class UTextComponent : public UBillboardComponent
{
    DECLARE_CLASS(UTextComponent, UBillboardComponent)

public:
    UTextComponent();

    //virtual UObject* Duplicate(UObject* InOuter) override;
    
    void GetProperties(TMap<FString, FString>& OutProperties) const override;
    
    void SetProperties(const TMap<FString, FString>& InProperties) override;

    virtual void InitializeComponent() override;
    
    virtual void TickComponent(float DeltaTime) override;
    
    void ClearText();
    
    void SetText(const FWString& InText);

    FWString GetText() { return Text; }
    
    void SetRowColumnCount(int CellsPerRow, int CellsPerColumn);

    virtual int CheckRayIntersection(const FVector& InRayOrigin, const FVector& InRayDirection, float& OutHitDistance) const override;
 
    
    float GetRowCount() const { return RowCount; }
    float GetColumnCount() const { return ColumnCount; }
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;

protected:

    // TODO: 씬저장에 FString로 저장되는 문제 있음
    FWString Text;

    //TArray<FVertexTexture> vertexTextureArr;

    UPROPERTY(int, QuadSize, = 2)

    UPROPERTY(int, RowCount, = 0)
    UPROPERTY(int, ColumnCount, = 0)
    
    UPROPERTY(int, QuadWidth, = 2.0f)
    UPROPERTY(int, QuadHeight, = 2.0f)

private:
    //FString TextAtlasBufferKey;
};
