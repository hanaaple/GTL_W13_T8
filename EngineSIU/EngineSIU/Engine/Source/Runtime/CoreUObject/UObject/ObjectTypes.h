#pragma once


enum class ESpawnObjectTypes : uint8
{
    Sphere,
    Cube,
    Spotlight,
    Pointlight,
    DirectionalLgiht,
    AmbientLight,
    Particle_Lagacy,
    ParticleSystem,
    TextRender,
    OBJ_CAMERA,
    OBJ_PLAYER,
    Fog,
    BoxCollision,
    SphereCollision,
    CapsuleCollision,
    SkeletalMeshActor,
    // OBJ_SEQUENCERPLAYER,
    PlayerStart,
    OBJ_END
};

enum ARROW_DIR : uint8
{
    AD_X,
    AD_Y,
    AD_Z,
    AD_END
};

enum EControlMode : uint8
{
    CM_TRANSLATION,
    CM_ROTATION,
    CM_SCALE,
    CM_END
};

enum ECoordMode : uint8
{
    CDM_WORLD,
    CDM_LOCAL,
    CDM_END
};

enum EPrimitiveColor : uint8
{
    RED_X,
    GREEN_Y,
    BLUE_Z,
    NONE,
    RED_X_ROT,
    GREEN_Y_ROT,
    BLUE_Z_ROT
};
