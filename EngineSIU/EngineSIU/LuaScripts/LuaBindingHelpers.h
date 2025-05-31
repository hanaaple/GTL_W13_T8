
#pragma once
#include <sol/sol.hpp>
#include "Runtime/Core/Math/Vector.h"
#include "Runtime/Engine/UserInterface/Console.h"
#include "Developer/LuaUtils/LuaBindMacros.h"

#include "Engine/Engine.h"
#include "World/World.h"

namespace LuaBindingHelpers
{
    // FVector 타입 바인딩 함수
    inline void BindFVector(sol::state& Lua)
    {
        Lua.Lua_NewUserType(
            FVector,
    
            // Constructors
            LUA_BIND_CONSTRUCTORS(
                FVector(),
                FVector(float, float, float),
                FVector(float)
            ),
    
            // Member variables
            LUA_BIND_MEMBER(&FVector::X),
            LUA_BIND_MEMBER(&FVector::Y),
            LUA_BIND_MEMBER(&FVector::Z),
    
            // Utility functions
            LUA_BIND_FUNC(&FVector::Dot),
            LUA_BIND_FUNC(&FVector::Cross),
            LUA_BIND_FUNC(&FVector::Equals),
            LUA_BIND_FUNC(&FVector::AllComponentsEqual),
            LUA_BIND_FUNC(&FVector::Length),
            LUA_BIND_FUNC(&FVector::SquaredLength),
            LUA_BIND_FUNC(&FVector::SizeSquared),
            LUA_BIND_FUNC(&FVector::Normalize),
            LUA_BIND_FUNC(&FVector::GetUnsafeNormal),
            LUA_BIND_FUNC(&FVector::GetSafeNormal),
            LUA_BIND_FUNC(&FVector::ComponentMin),
            LUA_BIND_FUNC(&FVector::ComponentMax),
            LUA_BIND_FUNC(&FVector::IsNearlyZero),
            LUA_BIND_FUNC(&FVector::IsZero),
            LUA_BIND_FUNC(&FVector::IsNormalized),
            LUA_BIND_FUNC(&FVector::ToString),
    
            // Static Method
            LUA_BIND_FUNC(&FVector::Zero),
            LUA_BIND_FUNC(&FVector::One),
    
            LUA_BIND_FUNC(&FVector::UnitX),
            LUA_BIND_FUNC(&FVector::UnitY),
            LUA_BIND_FUNC(&FVector::UnitZ),
    
            LUA_BIND_FUNC(&FVector::Distance),
            LUA_BIND_FUNC(&FVector::DotProduct),
            LUA_BIND_FUNC(&FVector::CrossProduct),
    
            // Static properties
            LUA_BIND_STATIC(FVector::ZeroVector),
            LUA_BIND_STATIC(FVector::OneVector),
            LUA_BIND_STATIC(FVector::UpVector),
            LUA_BIND_STATIC(FVector::DownVector),
            LUA_BIND_STATIC(FVector::ForwardVector),
            LUA_BIND_STATIC(FVector::BackwardVector),
            LUA_BIND_STATIC(FVector::RightVector),
            LUA_BIND_STATIC(FVector::LeftVector),
            LUA_BIND_STATIC(FVector::XAxisVector),
            LUA_BIND_STATIC(FVector::YAxisVector),
            LUA_BIND_STATIC(FVector::ZAxisVector),
    
            // Operators
            sol::meta_function::addition, LUA_BIND_OVERLOAD_WITHOUT_NAME2(\
                &FVector::operator+,
                FVector(const FVector&) const,
                FVector(float) const
            ),
            sol::meta_function::subtraction, LUA_BIND_OVERLOAD_WITHOUT_NAME2(
                &FVector::operator-,
                FVector(const FVector&) const,
                FVector(float) const
            ),
            sol::meta_function::multiplication, LUA_BIND_OVERLOAD_WITHOUT_NAME2(
                &FVector::operator*,
                FVector(const FVector&) const,
                FVector(float) const
            ),
            sol::meta_function::division, LUA_BIND_OVERLOAD_WITHOUT_NAME2(
                &FVector::operator/,
                FVector(const FVector&) const,
                FVector(float) const
            ),
            sol::meta_function::equal_to, &FVector::operator==,
    
            // 연산자 메서드
            "AddAssign", [](FVector& Self, const FVector& Other) { return Self += Other; },
            "SubAssign", [](FVector& Self, const FVector& Other) { return Self -= Other; },
            "MulAssign", [](FVector& Self, float Scalar) { return Self *= Scalar; },
            "DivAssign", [](FVector& Self, float Scalar) { return Self /= Scalar; }
        );
    }

    inline void BindFRotator(sol::state& Lua)
    {
        Lua.new_usertype<FRotator>("FRotator",
            // 생성자
            sol::constructors<
                FRotator(), 
                FRotator(float, float, float)
            >(),

            // 속성
            "Pitch", &FRotator::Pitch,
            "Yaw",   &FRotator::Yaw,
            "Roll",  &FRotator::Roll,

            // 연산자
            sol::meta_function::addition,      [](const FRotator& a, const FRotator& b){ return a + b; },
            sol::meta_function::subtraction,   [](const FRotator& a, const FRotator& b){ return a - b; }
        );
    }
    
    // UE_LOG 바인딩 함수
    inline void BindUE_LOG(sol::state& Lua)
    {
        Lua.set_function("UE_LOG",
            [](const std::string& Level, const std::string& Msg)
            {
                FString Converted = FString(Msg.c_str());
                if (Level == "Error")
                {
                    UE_LOG(ELogLevel::Error, TEXT("%s"), *Converted);
                }
                else if (Level == "Warning")
                {
                    UE_LOG(ELogLevel::Warning, TEXT("%s"), *Converted);
                }
                else
                {
                    UE_LOG(ELogLevel::Display, TEXT("%s"), *Converted);
                }
            }
        );
    }

    // Lua print 함수 바인딩 (콘솔 + 화면)
    inline void BindPrint(sol::state& Lua)
    {
        Lua.set_function("print",
            [](const std::string& Msg)
            {
                // 로그에 출력
                UE_LOG(ELogLevel::Error, TEXT("%s"), Msg.c_str());
                // 화면에 출력
                OutputDebugStringA(Msg.c_str()); // 디버그 창에 출력
            }
        );
    }

    inline void BindController(sol::state& Lua)
    {
        Lua.set_function("controller",
            [](const std::string& Key, AActor* LuaObj, const std::function<void(float)>& Callback)
            {
                //FString 주면 됨
                if (GEngine->ActiveWorld->GetPlayerController())
                {
                    return GEngine->ActiveWorld->GetPlayerController()->BindLuaAction(FString(Key), LuaObj, Callback);
                }

                return UINT64_MAX;
            }
        );
    }

    inline void BindUnBindController(sol::state& Lua)
    {
        Lua.set_function("unBindController",
            [](const std::string& Key, uint64 HandleId)
            {
                //FString 주면 됨
                if (GEngine->ActiveWorld->GetPlayerController())
                {
                    GEngine->ActiveWorld->GetPlayerController()->UnBindLuaAction(FString(Key),HandleId);
                }
            }
        );
    }
}

namespace LuaDebugHelper
{
    /**
     * @brief 바인딩 전 sol::state 의 globals() 키를 TArray<FString>로 캡처
     */
    inline TArray<FString> CaptureGlobalNames(sol::state& Lua)
    {
        TArray<FString> names;
        sol::table G = Lua.globals();
        for (auto& kv : G)
        {
            if (kv.first.is<std::string>())
            {
                names.Add(FString(kv.first.as<std::string>().c_str()));
            }
        }
        return names;
    }

    /**
     * @brief 바인딩 후 globals()에서 before에 없던 새 키만 로그로 출력
     */
    inline void LogNewBindings(sol::state& Lua, const TArray<FString>& Before)
    {
        sol::table G = Lua.globals();
        for (auto& kv : G)
        {
            if (!kv.first.is<std::string>())
                continue;

            FString name = FString(kv.first.as<std::string>().c_str());
            // Before 배열에 포함되지 않은 경우만 출력
            if (!Before.Contains(name))
            {
                UE_LOG(ELogLevel::Error,TEXT("Lua binding added: %s"), *name);
            }
        }
    }
}
