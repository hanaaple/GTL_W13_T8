#include "SpringArmComponent.h"

#include "Actor.h"
#include "HAL/PlatformType.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "UObject/UObjectIterator.h"
#include "World/World.h"

USpringArmComponent::USpringArmComponent()
{
    // SetRelativeRotation(FRotator(FVector(-3, -14, -5)));

    TargetArmLength = 5.f;
    TargetOffset = FVector(-13.f, 0.f, 4.f); // 부모에 대한 상대 위치

    bUsePawnControlRotation = true;
    bDoCollisionTest = false;

    bInheritPitch = bInheritYaw = bInheritRoll = true;
    bEnableCameraLag = true;
    bEnableCameraRotationLag = true;
    bUseCameraLagSubstepping = true;

    ProbeSize = 0.3f;
    CameraLagSpeed = 10.f;
    CameraRotationLagSpeed = 10.f;
    CameraLagMaxTimeStep = 1.f / 60.f;
    CameraLagMaxDistance = 0.f;

    bAbsoluteRotation = false;
    
}

FRotator USpringArmComponent::GetDesiredRotation() const
{
    return GetComponentRotation();
}

FRotator USpringArmComponent::GetTargetRotation() const
{
    FRotator DesiredRot = GetDesiredRotation();

    /* bUsePawnControlRotation : 액터의 ViewRotation을 기준으로 삼음 */
    if (AActor* OwningActor = Cast<AActor>(GetOwner()))
    {
        if (bUsePawnControlRotation)
        {
            const FRotator ActorViewRoation = OwningActor->GetActorRotation();
            DesiredRot = ActorViewRoation;
        }
    }

    /* 만일 월드 기준 절대 회전 값이 아닌 상대 회전 값을 쓴다면
     * binheritPitch, bInheritYaw, bInheritRoll를 검사하여
     * 일부 회전 값만 부모에서 가져오고, 나머지는 본인의 로컬 회전 사용
     * ex) bInheritYaw == false : 부모의 Yaw 회전 무시하고, 본인의 Yaw 회전 사용
     */
    if (!IsUsingAbsoluteRotation())
    {
        const FRotator LocalRelativeRotation = GetRelativeRotation();
        if (!bInheritPitch)
        {
            DesiredRot.Pitch = LocalRelativeRotation.Pitch;
        }
        if (!bInheritYaw)
        {
            DesiredRot.Yaw = LocalRelativeRotation.Yaw;
        }
        if (!bInheritRoll)
        {
            DesiredRot.Roll = LocalRelativeRotation.Roll;
        }
    }

    return DesiredRot;
}


void USpringArmComponent::TickComponent(float DeltaTime)
{

    //Super::TickComponent(DeltaTime);
    UpdateDesiredArmLocation(bDoCollisionTest, bEnableCameraLag, bEnableCameraRotationLag, DeltaTime);
}

void USpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
    FRotator DesiredRot = GetTargetRotation();
    //UE_LOG(ELogLevel::Display, TEXT("Target Rotation : %.2f %.2f %.2f"), DesiredRot.Yaw, DesiredRot.Pitch, DesiredRot.Roll);

    /* 회전 지연 여부 검사 및 반영 : bDoRotationLag */
    /* 서브스텝 분기 및 단일 스텝 분기 (DeltaTime 전체를 여러 LagMaxTimeStep이하 조각으로 나누고, 각 조각마다 RInterTo를 호출 */
    if (bDoRotationLag)
    {
        if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
        {
            const FRotator ArmRotStep = (DesiredRot - PreviousDesiredRot).GetNormalized() * (1.f / DeltaTime);
            FRotator LerpTarget = PreviousDesiredRot;
            float RemainingTime = DeltaTime;
            while (RemainingTime > KINDA_SMALL_NUMBER)
            {
                const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
                LerpTarget += ArmRotStep * LerpAmount;
                RemainingTime -= LerpAmount;

                DesiredRot = FMath::RInterpTo(PreviousDesiredRot, LerpTarget, LerpAmount, CameraRotationLagSpeed);
                PreviousDesiredRot = DesiredRot;
            }
        }
        else
        {
            DesiredRot = FMath::RInterpTo(PreviousDesiredRot, DesiredRot, DeltaTime, CameraRotationLagSpeed);
        }
    }
    PreviousDesiredRot = DesiredRot;

    /* 위치 지연 여부 검사 및 반영 : bDoLocationLag */
    

    /* 카메라 포지션으로부터 회전을 반영한 TargetOffset 만큼 떨어짐  */
    FVector RotatedOffset = DesiredRot.RotateVector(TargetOffset);
    FVector ArmOrigin = GetOwner()->GetActorLocation() + RotatedOffset;
    FVector DesiredLoc = ArmOrigin;


    if (bDoLocationLag)
    {
        if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
        {
            const FVector ArmMovementStep = (DesiredLoc - PreviousDesiredLoc) * (1.f / DeltaTime);
            FVector LerpTarget = PreviousDesiredLoc;

            float RemainingTime = DeltaTime;
            while (RemainingTime > KINDA_SMALL_NUMBER)
            {
                const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
                LerpTarget += ArmMovementStep * LerpAmount;
                RemainingTime -= LerpAmount;

                DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
                PreviousDesiredLoc = DesiredLoc;
            }
        }
        else
        {
            DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
        }
    }
    //PreviousArmOrigin = ArmOrigin;
    //PreviousDesiredLoc = DesiredLoc; // 예외 처리일라나?

    DesiredLoc -= DesiredRot.ToVector() * TargetArmLength;
    //DesiredLoc += FMatrix::GetRotationMatrix(DesiredRot).TransformVector(SocketOffset); // [unused] 로컬 공간 오프셋 추가

    //FTransform WorldCamTM;
    // 반영 및 child transform 반영


    FVector ResultLoc;
    /* 충돌 검사 및 암 길이 줄일지 여부 결정 : bDoTrace */
    if (bDoTrace)
    {
        FVector RayStart = GetOwner()->GetActorLocation() + FVector(0,0, TargetOffset.Z);
        FVector RayEnd = ArmOrigin + FVector(0, 0, TargetOffset.Z);
        FVector RayDelta = RayEnd - RayStart;
        float RayLen = RayDelta.Length();

        if (RayLen < KINDA_SMALL_NUMBER)
        {
            ResultLoc = DesiredLoc;
        }
        else
        {
            FVector RayDir = RayDelta / RayLen;
            //UE_LOG(ELogLevel::Error, "Ray direction : %.2f %.2f %.2f", RayDir.X, RayDir.Y, RayDir.Z);
            bool bHitSomething = false;
            float ClosestT = 1.f;               // 가중치 (0: 시작점, 1: 끝점)
            FVector BestHitWorld = DesiredLoc;

            for (UBoxComponent* BoxComp : TObjectRange<UBoxComponent>())
            {
                if (BoxComp->GetOwner() == GetOwner()) { continue; }

                float t;
                if (RaySweepBox(
                    RayStart,
                    RayDir,
                    RayLen,
                    BoxComp->GetWorldMatrix(),
                    BoxComp->GetBoxExtent(),
                    ProbeSize,
                    t))
                {
                    if (t < ClosestT)
                    {
                        ClosestT = t;
                        bHitSomething = true;
                        BestHitWorld = RayStart + RayDir * (t * RayLen - ProbeSize);
                        // 실제 충돌 지점으로 BestHitWorld 위치 조정
                    }
                }
            }

            if (bHitSomething)
            {
                ResultLoc = BlendLocations(DesiredLoc, BestHitWorld, bHitSomething, DeltaTime);
            }
            else
            {
                ResultLoc = DesiredLoc;
            }

        }
    }
    else
    {
        ResultLoc = DesiredLoc;
    }

    //UE_LOG(ELogLevel::Display, TEXT("Result Location : %.2f %.2f %.2f"), ResultLoc.X, ResultLoc.Y, ResultLoc.Z);
    SetWorldLocation(ResultLoc);
    SetWorldRotation(DesiredRot);
}

FVector USpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
    return bHitSomething ? TraceHitLocation : DesiredArmLocation;
}


bool USpringArmComponent::RaySweepBox(const FVector& Start, const FVector& Dir,
                                      float MaxDist, const FMatrix& BoxMatrix,
                                      const FVector& BoxExtents, float ProbeRadius, float& OutT)       // 0~1
{
    // 1) OBB 를 Probe만큼 확장한 AABB 로 변환
    FVector Extents = BoxExtents + FVector(ProbeRadius);

    // 2) 월드→박스 로컬
    FMatrix InvBox = FMatrix::Inverse(BoxMatrix);
    FVector LocalStart = InvBox.TransformPosition(Start);
    FVector LocalDir = FMatrix::TransformVector(Dir, InvBox);

    // 3) 슬랩(slab) Ray‐AABB
    float tMin = 0.f, tMax = MaxDist;
    for (int i = 0;i < 3;++i)
    {
        float origin = LocalStart[i];
        float dir = LocalDir[i];
        float e = Extents[i];
        if (FMath::Abs(dir) < SMALL_NUMBER)
        {
            if (origin < -e  || origin > e)
                return false;
        }
        else
        {
            float invD = 1.f / dir;
            float t1 = (-e - origin) * invD;
            float t2 = (e - origin) * invD;
            if (t1 > t2) std::swap(t1, t2);
            tMin = FMath::Max(tMin, t1);
            tMax = FMath::Min(tMax, t2);
            if (tMin > tMax) return false;
        }
    }

    if (tMin < 0.f || tMin > MaxDist)
        return false;

    OutT = tMin / MaxDist;
    return true;
}
