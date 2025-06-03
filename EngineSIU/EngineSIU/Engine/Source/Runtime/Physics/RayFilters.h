#pragma once
#include <PxQueryFiltering.h>

class RayFilterIgnoreActor: public physx::PxQueryFilterCallback
{
public:
    RayFilterIgnoreActor(PxRigidActor* ignoreActor)
        : actorToIgnore(ignoreActor) {}

    virtual physx::PxQueryHitType::Enum preFilter(
        const physx::PxFilterData& filterData,
        const physx::PxShape* shape,
        const physx::PxRigidActor* actor,
        PxHitFlags& queryFlags
    ) {
        if ( actor == actorToIgnore )
            return physx::PxQueryHitType::eNONE; // 충돌 무시

        return physx::PxQueryHitType::eBLOCK; // 기본 처리
    }

    virtual physx::PxQueryHitType::Enum postFilter(
        const physx::PxFilterData&,
        const physx::PxQueryHit& hit
    ) {
        return physx::PxQueryHitType::eNONE; // 일반적으로 사용 안 함
    }

private:
    physx::PxRigidActor* actorToIgnore;
};
