#pragma once
#include"Components/SceneComponent.h"

class UProjectileMovementComponent : public USceneComponent
{
    DECLARE_CLASS(UProjectileMovementComponent, USceneComponent)
public:
    UProjectileMovementComponent();
    virtual ~UProjectileMovementComponent() override = default;

    //virtual UObject* Duplicate(UObject* InOuter) override;

    void SetVelocity(FVector NewVelocity) { Velocity = NewVelocity; }

    FVector GetVelocity() const { return Velocity; }

    void SetInitialSpeed(float NewInitialSpeed) { InitialSpeed = NewInitialSpeed; }

    float GetInitialSpeed() const { return InitialSpeed; }

    void SetMaxSpeed(float NewMaxSpeed) { MaxSpeed = NewMaxSpeed; }

    float GetMaxSpeed() const { return MaxSpeed; }

    void SetGravity(float NewGravity) { Gravity = NewGravity; }

    float GetGravity() const { return Gravity; }

    void SetLifetime(float NewLifetime) { ProjectileLifetime = NewLifetime; }

    float GetLifetime() const { return ProjectileLifetime; }

    virtual void BeginPlay() override;
    
    virtual void TickComponent(float DeltaTime) override;
    
    virtual void GetProperties(TMap<FString, FString>& OutProperties) const override;
    virtual void SetProperties(const TMap<FString, FString>& InProperties) override;

private:

    UPROPERTY(EditAnywhere, float, ProjectileLifetime, = 10.0f)
    UPROPERTY(EditAnywhere, float, AccumulatedTime, = 0.0f)

    UPROPERTY(EditAnywhere, float, InitialSpeed, = 0.0f)
    UPROPERTY(EditAnywhere, float, MaxSpeed, = 0.0f)

    UPROPERTY(EditAnywhere, float, Gravity, = 0.0f)

    UPROPERTY(EditAnywhere, FVector, Velocity, = FVector::ZeroVector)
};

