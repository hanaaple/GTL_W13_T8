#include "Quat.h"

#include "Vector.h"
#include "Matrix.h"

const FQuat FQuat::Identity = FQuat{0.0f, 0.0f, 0.0f, 1.0f};

FQuat::FQuat(const FVector& Axis, float AngleRad)
{
    *this = FromAxisAngle(Axis, AngleRad);
}

FQuat::FQuat(const FRotator& R)
{
    // FRotator의 Pitch, Yaw, Roll (도 단위)을 사용
    // 언리얼의 FRotator::Quaternion() 메서드 로직 참고 (Z-Y-X 순서로 회전 적용)
    // 오일러 각도에서 쿼터니언으로 변환하는 일반적인 공식 사용
    // constexpr float DegreeToRadian = PI / 180.f;
    // const float PitchRad = R.Pitch * DegreeToRadian * 0.5f;
    // const float YawRad   = R.Yaw   * DegreeToRadian * 0.5f;
    // const float RollRad  = R.Roll  * DegreeToRadian * 0.5f;
    //
    // const float SP = FMath::Sin(PitchRad);
    // const float CP = FMath::Cos(PitchRad);
    // const float SY = FMath::Sin(YawRad);
    // const float CY = FMath::Cos(YawRad);
    // const float SR = FMath::Sin(RollRad);
    // const float CR = FMath::Cos(RollRad);

    // ZYX 순서 적용 (Yaw -> Pitch -> Roll)
    // W = CR*CP*CY + SR*SP*SY;
    // X = SR*CP*CY - CR*SP*SY;
    // Y = CR*SP*CY + SR*CP*SY;
    // Z = CR*CP*SY - SR*SP*CY;
    // 위는 일반적인 공식. 언리얼은 FQuat(Pitch,0,0) * FQuat(0,Yaw,0) * FQuat(0,0,Roll) 과 유사하게 계산
    // MakeFromEuler 참조
    *this = MakeFromEuler(FVector(R.Roll, R.Pitch, R.Yaw));
}

FQuat::FQuat(const FMatrix& M)
{
    // 언리얼 엔진의 FQuat(const FMatrix& M) 생성자 로직 참고
    // (M.M[row][col] 형태의 접근을 가정)

    // 행렬의 트레이스(대각합)를 확인
    const float Trace = M.M[0][0] + M.M[1][1] + M.M[2][2];

    if (Trace > 0.0f)
    {
        const float S = FMath::Sqrt(Trace + 1.0f) * 2.0f; // S=4*qw
        W = 0.25f * S;
        X = (M.M[1][2] - M.M[2][1]) / S;
        Y = (M.M[2][0] - M.M[0][2]) / S;
        Z = (M.M[0][1] - M.M[1][0]) / S;
    }
    else if ((M.M[0][0] > M.M[1][1]) && (M.M[0][0] > M.M[2][2])) // M.M[0][0] is largest
    {
        const float S = FMath::Sqrt(1.0f + M.M[0][0] - M.M[1][1] - M.M[2][2]) * 2.0f; // S=4*qx
        W = (M.M[1][2] - M.M[2][1]) / S;
        X = 0.25f * S;
        Y = (M.M[1][0] + M.M[0][1]) / S;
        Z = (M.M[2][0] + M.M[0][2]) / S;
    }
    else if (M.M[1][1] > M.M[2][2]) // M.M[1][1] is largest
    {
        const float S = FMath::Sqrt(1.0f + M.M[1][1] - M.M[0][0] - M.M[2][2]) * 2.0f; // S=4*qy
        W = (M.M[2][0] - M.M[0][2]) / S;
        X = (M.M[1][0] + M.M[0][1]) / S;
        Y = 0.25f * S;
        Z = (M.M[2][1] + M.M[1][2]) / S;
    }
    else // M.M[2][2] is largest
    {
        const float S = FMath::Sqrt(1.0f + M.M[2][2] - M.M[0][0] - M.M[1][1]) * 2.0f; // S=4*qz
        W = (M.M[0][1] - M.M[1][0]) / S;
        X = (M.M[2][0] + M.M[0][2]) / S;
        Y = (M.M[2][1] + M.M[1][2]) / S;
        Z = 0.25f * S;
    }
}

FQuat FQuat::FindBetween(const FVector& A, const FVector& B)
{
    const float NormAB = FMath::Sqrt(A.SizeSquared() * B.SizeSquared());
    float W = NormAB + FVector::DotProduct(A, B);
    FQuat Result;

    if (W >= 1e-6f * NormAB)
    {
        // Result = FVector::CrossProduct(A, B);
        Result = FQuat(
            A.Y * B.Z - A.Z * B.Y,
            A.Z * B.X - A.X * B.Z,
            A.X * B.Y - A.Y * B.X,
            W
        );
    }
    else
    {
        // A and B point in opposite directions
        W = 0.f;
        const float X = FMath::Abs(A.X);
        const float Y = FMath::Abs(A.Y);
        const float Z = FMath::Abs(A.Z);

        // Find orthogonal basis. 
        const FVector Basis = (X > Y && X > Z) ? FVector::YAxisVector : -FVector::XAxisVector;

        // Result = FVector::CrossProduct(A, Basis);
        Result = FQuat(
            A.Y * Basis.Z - A.Z * Basis.Y,
            A.Z * Basis.X - A.X * Basis.Z,
            A.X * Basis.Y - A.Y * Basis.X,
            W
        );
    }

    Result.Normalize();
    return Result;
}

FQuat FQuat::operator*(const FQuat& Other) const
{
    // (Q1 * Q2).W = (W1*W2 - X1*X2 - Y1*Y2 - Z1*Z2)
    // (Q1 * Q2).X = (W1*X2 + X1*W2 + Y1*Z2 - Z1*Y2)
    // (Q1 * Q2).Y = (W1*Y2 - X1*Z2 + Y1*W2 + Z1*X2)
    // (Q1 * Q2).Z = (W1*Z2 + X1*Y2 - Y1*X2 + Z1*W2)
    return FQuat{
        W * Other.X + X * Other.W + Y * Other.Z - Z * Other.Y,  // New X
        W * Other.Y - X * Other.Z + Y * Other.W + Z * Other.X,  // New Y
        W * Other.Z + X * Other.Y - Y * Other.X + Z * Other.W,  // New Z
        W * Other.W - X * Other.X - Y * Other.Y - Z * Other.Z   // New W
    };
}

FQuat FQuat::operator*(float Scale) const
{
    return FQuat{
        Scale * X,
        Scale * Y,
        Scale * Z,
        Scale * W
    };
}

float FQuat::operator|(const FQuat& Other) const
{
    return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
}

bool FQuat::operator==(const FQuat& Q) const
{
    return Equals(Q, SMALL_NUMBER);
}

FVector FQuat::RotateVector(const FVector& V) const
{
    // Q * V * Q.Conjugate()
    // V_quat = (V.X, V.Y, V.Z, 0)
    const FQuat Conjugate = FQuat(-X, -Y, -Z, W); // 쿼터니언의 켤레
    const FQuat VQuat(V.X, V.Y, V.Z, 0.f);
    const FQuat Temp = *this * VQuat;
    const FQuat Result = Temp * Conjugate;

    return FVector{Result.X, Result.Y, Result.Z};
}

bool FQuat::IsNormalized() const
{
    return FMath::Abs(X * X + Y * Y + Z * Z + W * W - 1.0f) < KINDA_SMALL_NUMBER; // 언리얼은 THRESH_QUAT_NORMALIZED 사용
}

void FQuat::Normalize(float Tolerance)
{
    // TODO: 추후에 SIMD 사용
    const float SquareSum = X * X + Y * Y + Z * Z + W * W;

    if (SquareSum >= Tolerance)
    {
        const float Scale = FMath::InvSqrt(SquareSum);
        X *= Scale;
        Y *= Scale;
        Z *= Scale;
        W *= Scale;
    }
    else
    {
        *this = Identity;
    }
}

void FQuat::ToAxisAndAngle(FVector& Axis, float& Angle) const
{
    Angle = (float)GetAngle();  // 각도 추출
    Axis = GetRotationAxis();   // 축 벡터 계산
}

float FQuat::GetAngle() const
{
    // W 값은 [-1, 1] 범위로 clamp
    return 2.0f * FMath::Acos(FMath::Clamp(W, -1.f, 1.f));
}

FVector FQuat::GetRotationAxis() const
{
    // TODO: 추후에 SIMD 사용

    // 벡터 성분의 제곱합 계산
    const float SquareSum = X*X + Y*Y + Z*Z;
    if (SquareSum < SMALL_NUMBER)
    {
        return FVector::XAxisVector;
    }

    // 벡터 정규화
    const float Scale = FMath::InvSqrt(SquareSum);
    return FVector{X * Scale, Y * Scale, Z * Scale};
}

bool FQuat::ContainsNaN() const
{
    return FMath::IsNaN(X) || FMath::IsNaN(Y) || FMath::IsNaN(Z) || FMath::IsNaN(W);
}

FQuat FQuat::Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
{
    // Get cosine of angle between quats.
    float RawCosom =
        Quat1.X * Quat2.X +
        Quat1.Y * Quat2.Y +
        Quat1.Z * Quat2.Z +
        Quat1.W * Quat2.W;

    // Unaligned quats - compensate, results in taking shorter route.
    const float Sign = FMath::FloatSelect(RawCosom, 1.0f, -1.0f);
    RawCosom *= Sign;
		
    float Scale0 = 1.0f - Slerp;
    float Scale1 = Slerp * Sign;
		
    if (RawCosom < 0.9999)
    {
        const float Omega = FMath::Acos(RawCosom);
        const float InvSin = 1.0f / FMath::Sin(Omega);
        Scale0 = FMath::Sin(Scale0 * Omega) * InvSin;
        Scale1 = FMath::Sin(Scale1 * Omega) * InvSin;
    }
		
    return FQuat{
        Scale0 * Quat1.X + Scale1 * Quat2.X,
        Scale0 * Quat1.Y + Scale1 * Quat2.Y,
        Scale0 * Quat1.Z + Scale1 * Quat2.Z,
        Scale0 * Quat1.W + Scale1 * Quat2.W
    };
}

FQuat FQuat::FromAxisAngle(const FVector& Axis, float AngleRad)
{
    const float HalfAngle = AngleRad * 0.5f;
    const float SinHalfAngle = FMath::Sin(HalfAngle);
    const float CosHalfAngle = FMath::Cos(HalfAngle);

    // Axis는 정규화되어 있어야 함
    return FQuat{
        Axis.X * SinHalfAngle,
        Axis.Y * SinHalfAngle,
        Axis.Z * SinHalfAngle,
        CosHalfAngle
    };
}

FQuat FQuat::MakeFromEuler(const FVector& EulerDegrees)
{
    // Roll (X), Pitch (Y), Yaw (Z) 순서로 도(degree) 단위 입력
    // 언리얼 엔진의 FQuat(FRotator) 또는 FRotator::Quaternion() 로직 참고
    // 일반적인 오일러 각 -> 쿼터니언 변환 (Z-Y-X 순서의 회전 적용)

    constexpr float DegreeToRadian = PI / 180.f;
    const float HalfRoll  = (EulerDegrees.X * DegreeToRadian) * 0.5f; // X축 회전 (Roll)
    const float HalfPitch = (EulerDegrees.Y * DegreeToRadian) * 0.5f; // Y축 회전 (Pitch)
    const float HalfYaw   = (EulerDegrees.Z * DegreeToRadian) * 0.5f; // Z축 회전 (Yaw)

    const float SR = FMath::Sin(HalfRoll);  const float CR = FMath::Cos(HalfRoll);
    const float SP = FMath::Sin(HalfPitch); const float CP = FMath::Cos(HalfPitch);
    const float SY = FMath::Sin(HalfYaw);   const float CY = FMath::Cos(HalfYaw);

    // ZYX 순서 (Yaw, Pitch, Roll)
    // W = CR*CP*CY + SR*SP*SY;
    // X = SR*CP*CY - CR*SP*SY;
    // Y = CR*SP*CY + SR*CP*SY;
    // Z = CR*CP*SY - SR*SP*CY;
    // 위 공식은 월드축 기준 Z, 그 다음 Y, 그 다음 X 순서로 곱했을 때의 결과
    // FQuat q_yaw(0,0,SY,CY), q_pitch(0,SP,0,CP), q_roll(SR,0,0,CR);
    // return q_yaw * q_pitch * q_roll;
    // 이 순서는 회전을 적용하는 순서가 Yaw(Z), then Pitch(Y), then Roll(X) 임을 의미.
    
    return FQuat{
        CR*SP*CY + SR*CP*SY, // X  (Y*Z + X) - SY*CP*SR
        CR*CP*SY - SR*SP*CY, // Y  (Z - X*Y) - CY*SP*SR
        SR*CP*CY - CR*SP*SY, // Z  (X*Y*Z - Z) - CY*CP*SR
        CR*CP*CY + SR*SP*SY  // W
    };
    // 언리얼 엔진 소스 (Rotator.cpp -> FRotator::Quaternion())
    // W = CR * CP * CY + SR * SP * SY;
    // X = SR * CP * CY - CR * SP * SY;
    // Y = CR * SP * CY + SR * CP * SY;
    // Z = CR * CP * SY - SR * SP *CY;
    // 순서가 약간 다릅니다. 위 코드는 (X,Y,Z,W) 순서로 리턴하기 위함입니다.
    // 언리얼 엔진은 W가 마지막 멤버가 아니었을 시절부터 내려온 공식일 수 있습니다.
    // (float X, float Y, float Z, float W) 순서의 FQuat를 반환한다고 할 때:
    // X = SR * CP * CY - CR * SP * SY;
    // Y = CR * SP * CY + SR * CP * SY;
    // Z = CR * CP * SY - SR * SP * CY;
    // W = CR * CP * CY + SR * SP * SY;
    // 위 공식이 맞습니다.
}

FMatrix FQuat::ToMatrix() const
{
    FMatrix R;

    const float X2 = X + X;    const float Y2 = Y + Y;    const float Z2 = Z + Z;
    const float XX = X * X2;   const float XY = X * Y2;   const float XZ = X * Z2;
    const float YY = Y * Y2;   const float YZ = Y * Z2;   const float ZZ = Z * Z2;
    const float WX = W * X2;   const float WY = W * Y2;   const float WZ = W * Z2;

    R.M[0][0] = 1.0f - (YY + ZZ);    R.M[1][0] = XY - WZ;               R.M[2][0] = XZ + WY;             R.M[3][0] = 0.0f;
    R.M[0][1] = XY + WZ;             R.M[1][1] = 1.0f - (XX + ZZ);      R.M[2][1] = YZ - WX;             R.M[3][1] = 0.0f;
    R.M[0][2] = XZ - WY;             R.M[1][2] = YZ + WX;               R.M[2][2] = 1.0f - (XX + YY);    R.M[3][2] = 0.0f;
    
    R.M[0][3] = 0.0f;                R.M[1][3] = 0.0f;                  R.M[2][3] = 0.0f;                R.M[3][3] = 1.0f;

    return R;
}

FRotator FQuat::Rotator() const
{
    const float SingularityTest = Z * X - W * Y;
    const float YawY = 2.f * (W * Z + X * Y);
    const float YawX = (1.f - 2.f * (FMath::Square(Y) + FMath::Square(Z)));

    // reference 
    // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

    // this value was found from experience, the above websites recommend different values
    // but that isn't the case for us, so I went through different testing, and finally found the case 
    // where both of world lives happily. 
    constexpr float SINGULARITY_THRESHOLD = 0.4999995f;
    constexpr float RAD_TO_DEG = (180.f / PI);
    float Pitch, Yaw, Roll;

    if (SingularityTest < -SINGULARITY_THRESHOLD)
    {
        Pitch = -90.f;
        Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
        Roll = FRotator::NormalizeAxis(-Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
    }
    else if (SingularityTest > SINGULARITY_THRESHOLD)
    {
        Pitch = 90.f;
        Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
        Roll = FRotator::NormalizeAxis(Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
    }
    else
    {
        Pitch = (FMath::Asin(2.f * SingularityTest) * RAD_TO_DEG);
        Yaw = (FMath::Atan2(YawY, YawX) * RAD_TO_DEG);
        Roll = (FMath::Atan2(-2.f * (W*X + Y*Z), (1.f - 2.f * (FMath::Square(X) + FMath::Square(Y)))) * RAD_TO_DEG);
    }

    FRotator RotatorFromQuat = FRotator(Pitch, Yaw, Roll);

    return RotatorFromQuat;
}

FQuat FQuat::Inverse() const
{
    return FQuat{-X, -Y, -Z, W};
}

FString FQuat::ToString() const
{
    return FString::Printf(TEXT("X=%.9f Y=%.9f Z=%.9f W=%.9f"), X, Y, Z, W);
}

bool FQuat::IsIdentity() const
{
    return X == 0.0f && Y == 0.0f && Z == 0.0f && W == 1.0f;
}

float FQuat::Error(const FQuat& Q1, const FQuat& Q2)
{
    const float cosom = FMath::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
    return (FMath::Abs(cosom) < 0.9999999f) ? FMath::Acos(cosom)*(1.f / PI) : 0.0f;
}
