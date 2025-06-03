#include "BodyInstance.h"
#include "Components/PrimitiveComponent.h"
#include "Physics/PhysicsManager.h"
#include "Physics/RayFilters.h"

FBodyInstance::FBodyInstance(UPrimitiveComponent* InOwner) : OwnerComponent(InOwner)
{
    
}

void FBodyInstance::UpdateBodyProperty() 
{
    physx::PxRigidDynamic* body = BIGameObject->DynamicRigidBody;

    // 질량과 관성.
    {
        body->setMass(MassInKg);
        physx::PxTransform offset = physx::PxTransform(COMNudge.X, COMNudge.Y, COMNudge.Z);
        body->setCMassLocalPose(offset);
        physx::PxVec3 scale = physx::PxVec3(InertiaTensorScale.X, InertiaTensorScale.Y, InertiaTensorScale.Z);
        body->setMassSpaceInertiaTensor(scale);
    }

    // 움직임 제한.
    {
        physx::PxRigidDynamicLockFlags flags = physx::PxRigidDynamicLockFlags();
        if ( bLockXTranslation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
        if ( bLockYTranslation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
        if ( bLockZTranslation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
        if ( bLockXRotation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
        if ( bLockYRotation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
        if ( bLockZRotation ) flags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

        body->setRigidDynamicLockFlags(flags);
    }

    // 댐핑.
    {
        body->setLinearDamping(LinearDamping);
        body->setAngularDamping(AngularDamping);
    }
}

void FBodyInstance::SetGameObject(GameObject* InGameObject)
{
    BIGameObject = InGameObject;
}

FVector FBodyInstance::GetBodyLocation() 
{
    physx::PxTransform tf = BIGameObject->DynamicRigidBody->getGlobalPose();
    return FVector(tf.p.x, tf.p.y, tf.p.z);
}

void FBodyInstance::SetBodyLocation(FVector& Location)
{
    physx::PxTransform tf = BIGameObject->DynamicRigidBody->getGlobalPose();
    tf.p.x = Location.X;
    tf.p.y = Location.Y;
    tf.p.z = Location.Z;
    BIGameObject->DynamicRigidBody->setGlobalPose(tf);
}

FRotator FBodyInstance::GetBodyRotation() 
{
    physx::PxTransform tf = BIGameObject->DynamicRigidBody->getGlobalPose();
    FQuat q(tf.q.x, tf.q.y, tf.q.z, tf.q.w);

    return FRotator(q);
}

void FBodyInstance::SetBodyRotation(FRotator& Rotation ) 
{
    FQuat q(Rotation);
    physx::PxTransform tf = BIGameObject->DynamicRigidBody->getGlobalPose();
    tf.q.x = q.X;
    tf.q.y = q.Y;
    tf.q.z = q.Z;
    tf.q.w = q.W;
    BIGameObject->DynamicRigidBody->setGlobalPose(tf);
}

FVector FBodyInstance::GetBodyVelocity()
{
    physx::PxVec3 v = BIGameObject->DynamicRigidBody->getLinearVelocity();
    return FVector(v.x, v.y, v.z);
}

void FBodyInstance::SetBodyVelocity(FVector& Velocity) 
{
    BIGameObject->DynamicRigidBody->setLinearVelocity(
        physx::PxVec3(Velocity.X, Velocity.Y, Velocity.Z)
    );
}

FRotator FBodyInstance::GetBodyAngularVelocity()
{
    physx::PxVec3 r = BIGameObject->DynamicRigidBody->getAngularVelocity();
    return FRotator(r.x, r.y, r.z);
}

void FBodyInstance::SetBodyAngularVelocity(FRotator& Rotation)
{
    BIGameObject->DynamicRigidBody->setAngularVelocity(
        physx::PxVec3(Rotation.Pitch, Rotation.Yaw, Rotation.Roll)
    );
}

void FBodyInstance::AddBodyVelocity(FVector& Velocity) 
{
    BIGameObject->DynamicRigidBody->addForce(
        physx::PxVec3(Velocity.X, Velocity.Y, Velocity.Z),
        physx::PxForceMode::eVELOCITY_CHANGE
    );
}

void FBodyInstance::AddBodyAccelation(FVector& Accelation) 
{
    BIGameObject->DynamicRigidBody->addForce(
        physx::PxVec3(Accelation.X, Accelation.Y, Accelation.Z),
        physx::PxForceMode::eACCELERATION
    );
}

void FBodyInstance::AddBodyForce(FVector& Force) 
{
    BIGameObject->DynamicRigidBody->addForce(
        physx::PxVec3(Force.X, Force.Y, Force.Z),
        physx::PxForceMode::eFORCE
    );
}

void FBodyInstance::AddBodyImpulse(FVector& Impluse) 
{
    BIGameObject->DynamicRigidBody->addForce(
        physx::PxVec3(Impluse.X, Impluse.Y, Impluse.Z),
        physx::PxForceMode::eIMPULSE
    );
}

bool FBodyInstance::RayCast(FVector& Direction, float Distance) 
{
    physx::PxRigidBody* body = BIGameObject->DynamicRigidBody;
    physx::PxVec3 dir = physx::PxVec3(Direction.X, Direction.Y, Direction.Z).getNormalized();

    RayFilterIgnoreActor filter = RayFilterIgnoreActor(body);
    physx::PxRaycastBuffer hit;
    PxQueryFilterData filterData;
    filterData.flags |= PxQueryFlag::ePREFILTER;
    bool isHit = body->getScene()->raycast(
        body->getGlobalPose().p,
        dir,
        Distance,
        hit,
        PxHitFlag::eDEFAULT,
        filterData,
        &filter
    );
    return hit.hasBlock;
}
