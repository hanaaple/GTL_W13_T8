#pragma once
#include "sol/sol.hpp"

namespace SolTypeBinding
{
    template<typename... Types>
    struct TypeList {};

    // PushBack
    template<typename List, typename NewType>
    struct PushBack;

    // Pushback
    template<typename... Types, typename NewType>
    struct PushBack<TypeList<Types...>, NewType> {
        using type = TypeList<Types..., NewType>;
    };

    // Base 클래스를 상속하는 모든 타입을 리스트로 모은다
    template<typename Derived, typename Base = void>
    struct InheritList;

    // Base 없는 경우 (Root 클래스)
    template<typename Derived>
    struct InheritList<Derived, void> {
        using type = TypeList<Derived>;
    };

    // Base 있는 경우 (Derived -> Base)
    template<typename Derived, typename Base>
    struct InheritList {
        using base_list = typename Base::InheritTypes;
        using type = typename PushBack<base_list, Derived>::type;
    };

    // to unpack types
    template<typename TypeList>
    struct TypeListToBases;

    // to unpack types
    template<typename... Types>
    struct TypeListToBases<TypeList<Types...>> {
        static auto Get() {
            return sol::bases<Types...>();
        }
    };

    // to check incomplete type
    template <typename T>
    constexpr bool IsCompleteType_v = requires { sizeof(T); };
}
