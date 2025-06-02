#pragma once
#include "MathUtility.h"
#include "Rotator.h"
#include "Serialization/Archive.h"

struct FVector;
struct FMatrix;

/**
 * 4차원 복소수(quaternion)를 표현하는 구조체입니다. 회전 변환에 주로 사용됩니다.
 */
struct alignas(16) FQuat
{
public:
    float X, Y, Z, W;

    using FReal = float;

    static const FQuat Identity;

public:
    // 기본 생성자 (항등 쿼터니언: X=0, Y=0, Z=0, W=1)
    FQuat()
        : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
    {}

    // X, Y, Z, W 값으로 초기화
    FQuat(float InX, float InY, float InZ, float InW)
        : X(InX), Y(InY), Z(InZ), W(InW)
    {}

    // 회전 축과 각도(라디안)를 받아서 FQuat 생성
    FQuat(const FVector& Axis, float AngleRad);

    // 회전 행렬로부터 FQuat 생성
    FQuat(const FMatrix& M);

    // 오일러 각(FRotator)으로부터 FQuat 생성 (Pitch, Yaw, Roll 순서로 도(degree) 단위 입력)
    explicit FQuat(const FRotator& R);


public:
    /**
     * 벡터 A에서 B로의 최소 회전을 표현하는 쿼터니언을 계산합니다.
     *
     * @param A 시작 벡터
     * @param B 목표 벡터
     * @return 정규화된 단위 쿼터니언
     *
     * @warning 입력 벡터가 영벡터인 경우 정의되지 않은 동작
     * @note 평행/반대 방향 벡터 처리:
     *  - 평행 시: 임의의 수직축 기반 회전
     *  - 반대 시: X/Y/Z 축 중 최대 요소 기반 직교축 사용
     */
    static FQuat FindBetween(const FVector& A, const FVector& B);

    // 쿼터니언의 곱셈 연산 (회전 결합)
    FQuat operator*(const FQuat& Other) const;
    FQuat operator*(float Scale) const;

    float operator|(const FQuat& Other) const;
    bool operator==(const FQuat& Q) const;

    // (쿼터니언) 벡터 회전
    FVector RotateVector(const FVector& V) const;

    // 단위 쿼터니언 여부 확인
    bool IsNormalized() const;

    // 쿼터니언 정규화 (단위 쿼터니언으로 만듬)
    void Normalize(float Tolerance = SMALL_NUMBER);

    /** 정규화된 쿼터니언을 가져옵니다. */
    FORCEINLINE FQuat GetNormalized(float Tolerance = SMALL_NUMBER) const
    {
        FQuat Result(*this);
        Result.Normalize(Tolerance);
        return Result;
    }

    /**
     * 쿼터니언을 회전축과 각도(라디안)로 분해합니다.
     * 
     * @param Axis 회전축 단위벡터
     * @param Angle 라디안 단위 회전각 (0 <= Angle <= 2π)
     */
    void ToAxisAndAngle(FVector& Axis, float& Angle) const;

    /**
    * 쿼터니언의 회전각을 라디안 단위로 반환합니다.
    * 
    * @return 0 ~ 2π 범위의 회전각
    * @note 내부 계산식: 2 * acos(W)
    */
    float GetAngle() const;

    /**
     * 쿼터니언의 회전축을 계산합니다.
     * 
     * @return 정규화된 회전축 단위벡터
     * @warning 영쿼터니언(W=±1) 입력 시 X축 반환
     */
    FVector GetRotationAxis() const;

    /** FQuat가 오차값 이내로 같은지 확인합니다. */
    FORCEINLINE bool Equals(const FQuat& Q, float Tolerance=KINDA_SMALL_NUMBER) const
    {
        // TODO: 나중에 SSE를 이용해서 최적화
        return (FMath::Abs(X - Q.X) <= Tolerance && FMath::Abs(Y - Q.Y) <= Tolerance && FMath::Abs(Z - Q.Z) <= Tolerance && FMath::Abs(W - Q.W) <= Tolerance)
            || (FMath::Abs(X + Q.X) <= Tolerance && FMath::Abs(Y + Q.Y) <= Tolerance && FMath::Abs(Z + Q.Z) <= Tolerance && FMath::Abs(W + Q.W) <= Tolerance);
    }

    /**
     * 다른 쿼터니언과의 각도 거리를 계산합니다.
     *
     * @param Q 비교 대상 쿼터니언
     * @return 두 쿼터니언의 회전 각 차이(라디안 단위, 0 ~ π 범위)
     *
     * @note 내부 계산식: acos(2*(Q·this)^2 - 1)
     */
    FORCEINLINE float AngularDistance(const FQuat& Q) const
    {
        // 내적 계산 (4차원 벡터)
        const float InnerProd = X*Q.X + Y*Q.Y + Z*Q.Z + W*Q.W;

        // 각도 변환: 2*cos²θ - 1 = cos(2θ) 항등식 활용
        return FMath::Acos(FMath::Clamp((2 * InnerProd * InnerProd) - 1.f, -1.f, 1.f));
    }

    bool ContainsNaN() const;

    static FQuat Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp);

    static FORCEINLINE FQuat Slerp(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
    {
        return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
    }

    // 회전 각도와 축으로부터 쿼터니언 생성 (axis-angle 방식)
    static FQuat FromAxisAngle(const FVector& Axis, float AngleRad);

    // 오일러 각(Roll, Pitch, Yaw - 도 단위)으로부터 쿼터니언 생성. 회전 순서는 Z(Yaw) -> Y(Pitch) -> X(Roll).
    static FQuat MakeFromEuler(const FVector& EulerDegrees); // EulerDegrees.X = Roll, .Y = Pitch, .Z = Yaw

    // 쿼터니언을 회전 행렬로 변환
    FMatrix ToMatrix() const;

    // 쿼터니언을 FRotator (오일러 각, 도 단위)로 변환
    FRotator Rotator() const;

    FQuat Inverse() const;

    FString ToString() const;

    bool IsIdentity() const;

    /**
     * Error measure (angle) between two quaternions, ranged [0..1].
     * Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though 
     * @note normalized input is expected.
     */
    static float Error(const FQuat& Q1, const FQuat& Q2);
};

inline FArchive& operator<<(FArchive& Ar, FQuat& Q)
{
    return Ar << Q.X << Q.Y << Q.Z << Q.W;
}
