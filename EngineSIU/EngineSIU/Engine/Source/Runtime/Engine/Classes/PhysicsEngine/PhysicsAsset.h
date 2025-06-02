#pragma once
#include "Container/Array.h"

#include <PxPhysicsAPI.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

class USkeletalMesh;
struct FConstraintSetup;

enum class EGeomType : uint8
{
    ESphere,
    EBox,
    ECapsule,
    MAX,
};

struct FKAggregateGeom
{
    TArray<physx::PxShape*> SphereElems;
    TArray<physx::PxShape*> BoxElems;
    TArray<physx::PxShape*> CapsuleElems;
};

enum class ERigidBodyType : uint8
{
    STATIC,
    DYNAMIC, 
    KINEMATIC
};

struct AggregateGeomAttributes
{
    DECLARE_STRUCT(AggregateGeomAttributes)
    AggregateGeomAttributes() = default;

    UPROPERTY_WITH_FLAGS(EditAnywhere, EGeomType, GeomType, = EGeomType::EBox)
    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, Offset)
    UPROPERTY_WITH_FLAGS(EditAnywhere, FRotator, Rotation)
    UPROPERTY_WITH_FLAGS(EditAnywhere, FVector, Extent, = FVector(1,1,1)) // Half Extent

    friend FArchive& operator<<(FArchive& Ar, AggregateGeomAttributes& Attributes);
};

class UBodySetupCore : public UObject
{
    DECLARE_CLASS(UBodySetupCore, UObject)
    
public:
    UBodySetupCore() = default;
    void SetBoneName(const FName& InBoneName) { BoneName = InBoneName; }

    UPROPERTY(FName, BoneName, = TEXT("None"))

    virtual void SerializeAsset(FArchive& Ar) override;
};

class UBodySetup : public UBodySetupCore
{
    DECLARE_CLASS(UBodySetup, UBodySetupCore)
    
public:
    UBodySetup() = default;

    // DisplayName = Primitives
    FKAggregateGeom AggGeom;

    UPROPERTY(EditAnywhere, TArray<AggregateGeomAttributes>, GeomAttributes, = {})

    UPROPERTY(
        EditAnywhere, ({ .ClampMin = 0.0f, .ClampMax = 1.0f }),
        float, StaticFriction, = 0.5
    )
    UPROPERTY(
        EditAnywhere, ({ .ClampMin = 0.0f, .ClampMax = 1.0f }),
        float, DynamicFriction, = 0.5
    )
    UPROPERTY(
        EditAnywhere, ({ .ClampMin = 0.0f, .ClampMax = 1.0f }),
        float, Restitution, = 0.5
    )
    
    virtual void SerializeAsset(FArchive& Ar) override;
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
};

class UPhysicsAsset : public UObject
{
    DECLARE_CLASS(UPhysicsAsset, UObject)
    
public:
    UPhysicsAsset() = default;

    UPROPERTY(EditAnywhere | ShallowCopy, USkeletalMesh*, PreviewSkeletalMesh, = nullptr)

    UPROPERTY(EditAnywhere, TArray<UBodySetup*>, BodySetups, ={})

    /**
     * 언리얼 엔진에서는 UPhysicsConstraintTemplate의 배열을 가지고있고,
     * UPhysicsConstraintTemplate에는 FConstraintInstance타입인
     * DefaultInstance를 가지고있음.
     * UPhysicsConstraintTemplate의 DEPRECATED된 멤버 변수들을 보면
     * FConstraintInstance의 멤버 변수와 겹치기 때문에 사실상 같은 정보로 판단하여
     * FConstraintInstance를 사용.
     */
    TArray<FConstraintSetup*> ConstraintSetups;

    virtual bool SetPreviewMesh(USkeletalMesh* PreviewMesh);
    virtual USkeletalMesh* GetPreviewMesh() const;

    virtual void SerializeAsset(FArchive& Ar) override;
    virtual void DuplicateSubObjects(const UObject* Source, UObject* InOuter, FObjectDuplicator& Duplicator) override;
};
