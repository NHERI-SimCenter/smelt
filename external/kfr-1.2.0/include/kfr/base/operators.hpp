/** @addtogroup math
 *  @{
 */
/*
  Copyright (C) 2016 D Levin (https://www.kfrlib.com)
  This file is part of KFR

  KFR is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KFR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KFR.

  If GPL is not suitable for your project, you must purchase a commercial license to use KFR.
  Buying a commercial license is mandatory as soon as you develop commercial activities without
  disclosing the source code of your own applications.
  See https://www.kfrlib.com for details.
 */
#pragma once

#include "function.hpp"
#include <algorithm>
#include <utility>

namespace kfr
{
namespace internal
{

template <typename T, typename ReduceFn>
CMT_INLINE T horizontal_impl(const vec<T, 1>& value, ReduceFn&&)
{
    return T(value[0]);
}

template <typename T, size_t N, typename ReduceFn, KFR_ENABLE_IF(N > 1 && is_poweroftwo(N))>
CMT_INLINE T horizontal_impl(const vec<T, N>& value, ReduceFn&& reduce)
{
    return horizontal_impl(reduce(low(value), high(value)), std::forward<ReduceFn>(reduce));
}
template <typename T, size_t N, typename ReduceFn, KFR_ENABLE_IF(N > 1 && !is_poweroftwo(N))>
CMT_INLINE T horizontal_impl(const vec<T, N>& value, ReduceFn&& reduce)
{
    const T initial = reduce(initialvalue<T>());
    return horizontal_impl(widen<next_poweroftwo(N)>(value, initial), std::forward<ReduceFn>(reduce));
}
}

template <typename T, size_t N, typename ReduceFn>
CMT_INLINE T horizontal(const vec<T, N>& value, ReduceFn&& reduce)
{
    return internal::horizontal_impl(value, std::forward<ReduceFn>(reduce));
}

template <typename T>
constexpr inline T add(const T& x)
{
    return x;
}

/**
 * @brief Returns sum of all the arguments passed to a function.
 */
template <typename T1, typename T2, typename... Ts, KFR_ENABLE_IF(is_numeric_args<T1, T2, Ts...>::value)>
constexpr inline common_type<T1, T2, Ts...> add(const T1& x, const T2& y, const Ts&... rest)
{
    return x + add(y, rest...);
}
template <typename T>
constexpr inline T add(initialvalue<T>)
{
    return T(0);
}
KFR_FN(add)

/**
 * @brief Returns template expression that returns sum of all the arguments passed to a function.
 */
template <typename... E, KFR_ENABLE_IF(is_input_expressions<E...>::value)>
CMT_INLINE internal::expression_function<fn::add, E...> add(E&&... x)
{
    return { fn::add(), std::forward<E>(x)... };
}

template <typename T1, typename T2>
constexpr inline common_type<T1, T2> sub(const T1& x, const T2& y)
{
    return x - y;
}
template <typename T>
constexpr inline T sub(initialvalue<T>)
{
    return T(0);
}
KFR_FN(sub)

template <typename E1, typename E2, KFR_ENABLE_IF(is_input_expressions<E1, E2>::value)>
CMT_INLINE internal::expression_function<fn::sub, E1, E2> sub(E1&& x, E2&& y)
{
    return { fn::sub(), std::forward<E1>(x), std::forward<E2>(y) };
}

template <typename T1>
constexpr inline T1 mul(const T1& x)
{
    return x;
}

/**
 * @brief Returns product of all the arguments passed to a function.
 */
template <typename T1, typename T2, typename... Ts>
constexpr inline common_type<T1, T2, Ts...> mul(const T1& x, const T2& y, const Ts&... rest)
{
    return x * mul(y, rest...);
}

template <typename T>
constexpr inline T mul(initialvalue<T>)
{
    return T(1);
}
KFR_FN(mul)

/**
 * @brief Returns template expression that returns product of all the arguments passed to a function.
 */
template <typename... E, KFR_ENABLE_IF(is_input_expressions<E...>::value)>
CMT_INLINE internal::expression_function<fn::mul, E...> mul(E&&... x)
{
    return { fn::mul(), std::forward<E>(x)... };
}

/**
 * @brief Returns square of x.
 */
template <typename T1, KFR_ENABLE_IF(is_numeric<T1>::value)>
constexpr inline T1 sqr(const T1& x)
{
    return x * x;
}
KFR_FN(sqr)

/**
 * @brief Returns template expression that returns square of x.
 */
template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::sqr, E1> sqr(E1&& x)
{
    return { fn::sqr(), std::forward<E1>(x) };
}

/**
 * @brief Returns cube of x.
 */
template <typename T1, KFR_ENABLE_IF(is_numeric<T1>::value)>
constexpr inline T1 cub(const T1& x)
{
    return sqr(x) * x;
}
KFR_FN(cub)

/**
 * @brief Returns template expression that returns cube of x.
 */
template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::cub, E1> cub(E1&& x)
{
    return { fn::cub(), std::forward<E1>(x) };
}

template <typename T, KFR_ENABLE_IF(is_numeric_args<T>::value)>
constexpr inline T pow2(const T& x)
{
    return sqr(x);
}

template <typename T, KFR_ENABLE_IF(is_numeric_args<T>::value)>
constexpr inline T pow3(const T& x)
{
    return cub(x);
}

template <typename T, KFR_ENABLE_IF(is_numeric_args<T>::value)>
constexpr inline T pow4(const T& x)
{
    return sqr(sqr(x));
}

template <typename T, KFR_ENABLE_IF(is_numeric_args<T>::value)>
constexpr inline T pow5(const T& x)
{
    return pow4(x) * x;
}
KFR_FN(pow2)
KFR_FN(pow3)
KFR_FN(pow4)
KFR_FN(pow5)

template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::pow2, E1> pow2(E1&& x)
{
    return { fn::pow2(), std::forward<E1>(x) };
}
template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::pow3, E1> pow3(E1&& x)
{
    return { fn::pow3(), std::forward<E1>(x) };
}
template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::pow4, E1> pow4(E1&& x)
{
    return { fn::pow4(), std::forward<E1>(x) };
}
template <typename E1, KFR_ENABLE_IF(is_input_expression<E1>::value)>
CMT_INLINE internal::expression_function<fn::pow5, E1> pow5(E1&& x)
{
    return { fn::pow5(), std::forward<E1>(x) };
}

/// Raise x to the power base \f$ x^{base} \f$
/// @code
/// CHECK( ipow( 10, 3 ) == 1000 );
/// CHECK( ipow( 0.5, 2 ) == 0.25 );
/// @endcode
template <typename T>
constexpr inline T ipow(const T& x, int base)
{
    T result = T(1);
    while (base)
    {
        if (base & 1)
            result *= x;
        base >>= 1;
        x *= x;
    }
    return result;
}
KFR_FN(ipow)

template <typename E1, typename E2, KFR_ENABLE_IF(is_input_expressions<E1, E2>::value)>
CMT_INLINE internal::expression_function<fn::ipow, E1, E2> ipow(E1&& x, E2&& b)
{
    return { fn::ipow(), std::forward<E1>(x), std::forward<E2>(b) };
}

/// Return square of the sum of all arguments
/// *Example*::
///
/// CHECK(sqrsum(1,2,3) == 36);
template <typename T1, typename... Ts>
constexpr inline common_type<T1, Ts...> sqrsum(const T1& x, const Ts&... rest)
{
    return sqr(add(x, rest...));
}

template <typename T1, typename T2>
constexpr inline common_type<T1, T2> sqrdiff(const T1& x, const T2& y)
{
    return sqr(x - y);
}
KFR_FN(sqrsum)
KFR_FN(sqrdiff)

/// Division
template <typename T1, typename T2, typename Tout = common_type<T1, T2>>
inline Tout div(const T1& x, const T2& y)
{
    return static_cast<Tout>(x) / static_cast<Tout>(y);
}
KFR_FN(div)

/// Remainder
template <typename T1, typename T2, typename Tout = common_type<T1, T2>>
inline Tout rem(const T1& x, const T2& y)
{
    return static_cast<Tout>(x) % static_cast<Tout>(y);
}
KFR_FN(rem)

/// Negation
template <typename T1>
inline T1 neg(const T1& x)
{
    return -x;
}
KFR_FN(neg)

/// Bitwise Not
template <typename T1>
inline T1 bitwisenot(const T1& x)
{
    return ~x;
}
KFR_FN(bitwisenot)

/// Bitwise And
template <typename T1, typename T2>
inline common_type<T1, T2> bitwiseand(const T1& x, const T2& y)
{
    return x & y;
}
template <typename T>
constexpr inline T bitwiseand(initialvalue<T>)
{
    return internal::allones<subtype<T>>;
}
KFR_FN(bitwiseand)

/// Bitwise And-Not
template <typename T1, typename T2>
inline common_type<T1, T2> bitwiseandnot(const T1& x, const T2& y)
{
    return x & ~y;
}
template <typename T>
constexpr inline T bitwiseandnot(initialvalue<T>)
{
    return internal::allones<subtype<T>>;
}
KFR_FN(bitwiseandnot)

/// Bitwise Or
template <typename T1, typename T2>
inline common_type<T1, T2> bitwiseor(const T1& x, const T2& y)
{
    return x | y;
}
template <typename T>
constexpr inline T bitwiseor(initialvalue<T>)
{
    return subtype<T>();
}
KFR_FN(bitwiseor)

/// Bitwise Xor (Exclusive Or)
template <typename T1, typename T2>
inline common_type<T1, T2> bitwisexor(const T1& x, const T2& y)
{
    return x ^ y;
}
template <typename T>
constexpr inline T bitwisexor(initialvalue<T>)
{
    return subtype<T>();
}
KFR_FN(bitwisexor)

/// Bitwise Left shift
template <typename T1, typename T2>
inline common_type<T1, T2> shl(const T1& left, const T2& right)
{
    return left << right;
}
KFR_FN(shl)

/// Bitwise Right shift
template <typename T1, typename T2>
inline common_type<T1, T2> shr(const T1& left, const T2& right)
{
    return left >> right;
}
KFR_FN(shr)

/// Bitwise Left Rotate
template <typename T1, typename T2>
inline common_type<T1, T2> rol(const T1& left, const T2& right)
{
    return shl(left, right) | shr(left, (static_cast<subtype<T1>>(typebits<T1>::bits) - right));
}
KFR_FN(rol)

/// Bitwise Right Rotate
template <typename T1, typename T2>
inline common_type<T1, T2> ror(const T1& left, const T2& right)
{
    return shr(left, right) | shl(left, (static_cast<subtype<T1>>(typebits<T1>::bits) - right));
}
KFR_FN(ror)

template <typename T1, typename T2>
inline common_type<T1, T2> equal(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x == y);
}
template <typename T1, typename T2>
inline common_type<T1, T2> notequal(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x != y);
}
template <typename T1, typename T2>
inline common_type<T1, T2> less(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x < y);
}
template <typename T1, typename T2>
inline common_type<T1, T2> greater(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x > y);
}
template <typename T1, typename T2>
inline common_type<T1, T2> lessorequal(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x <= y);
}
template <typename T1, typename T2>
inline common_type<T1, T2> greaterorequal(const T1& x, const T2& y)
{
    return bitcast<subtype<common_type<T1, T2>>>(x >= y);
}
KFR_FN(equal)
KFR_FN(notequal)
KFR_FN(less)
KFR_FN(greater)
KFR_FN(lessorequal)
KFR_FN(greaterorequal)

/// @brief Fused Multiply-Add
template <typename T1, typename T2, typename T3>
KFR_INTRIN constexpr common_type<T1, T2, T3> fmadd(const T1& x, const T2& y, const T3& z)
{
    return x * y + z;
}
/// @brief Fused Multiply-Sub
template <typename T1, typename T2, typename T3>
KFR_INTRIN constexpr common_type<T1, T2, T3> fmsub(const T1& x, const T2& y, const T3& z)
{
    return x * y - z;
}
KFR_FN(fmadd)
KFR_FN(fmsub)

/// @brief Linear blend of `x` and `y` (`c` must be in the range 0...+1)
/// Returns `x + ( y - x ) * c`
template <typename T1, typename T2, typename T3, KFR_ENABLE_IF(is_numeric_args<T1, T2, T3>::value)>
KFR_INTRIN constexpr common_type<T1, T2, T3> mix(const T1& c, const T2& x, const T3& y)
{
    return fmadd(c, y - x, x);
}

/// @brief Linear blend of `x` and `y` (`c` must be in the range -1...+1)
template <typename T1, typename T2, typename T3, KFR_ENABLE_IF(is_numeric_args<T1, T2, T3>::value)>
KFR_INTRIN constexpr common_type<T1, T2, T3> mixs(const T1& c, const T2& x, const T3& y)
{
    return mix(fmadd(c, 0.5, 0.5), x, y);
}
KFR_FN(mix)
KFR_FN(mixs)

template <typename E1, typename E2, typename E3, KFR_ENABLE_IF(is_input_expressions<E1, E2, E3>::value)>
CMT_INLINE internal::expression_function<fn::mix, E1, E2, E3> mix(E1&& c, E2&& x, E3&& y)
{
    return { fn::mix(), std::forward<E1>(c), std::forward<E2>(x), std::forward<E3>(y) };
}

template <typename E1, typename E2, typename E3, KFR_ENABLE_IF(is_input_expressions<E1, E2, E3>::value)>
CMT_INLINE internal::expression_function<fn::mixs, E1, E2, E3> mixs(E1&& c, E2&& x, E3&& y)
{
    return { fn::mixs(), std::forward<E1>(c), std::forward<E2>(x), std::forward<E3>(y) };
}

namespace internal
{

template <typename T1, typename T2>
constexpr CMT_INLINE common_type<T1, T2> horner(const T1&, const T2& c0)
{
    return c0;
}

template <typename T1, typename T2, typename T3, typename... Ts>
constexpr CMT_INLINE common_type<T1, T2, T3, Ts...> horner(const T1& x, const T2& c0, const T3& c1,
                                                           const Ts&... values)
{
    return fmadd(horner(x, c1, values...), x, c0);
}
}

/// @brief Calculate polynomial using Horner's method
///
/// ``horner(x, 1, 2, 3)`` is equivalent to \(3x^2 + 2x + 1\)
template <typename T1, typename... Ts>
constexpr CMT_INLINE common_type<T1, Ts...> horner(const T1& x, const Ts&... c)
{
    return internal::horner(x, c...);
}
KFR_FN(horner)

/// @brief Calculate Multiplicative Inverse of `x`
/// Returns `1/x`
template <typename T>
constexpr CMT_INLINE T reciprocal(const T& x)
{
    static_assert(std::is_floating_point<subtype<T>>::value, "T must be floating point type");
    return subtype<T>(1) / x;
}
KFR_FN(reciprocal)

template <typename T1, typename T2>
CMT_INLINE common_type<T1, T2> mulsign(const T1& x, const T2& y)
{
    return x ^ (y & internal::highbitmask<subtype<T2>>);
}
KFR_FN(mulsign)

template <typename T, size_t N>
constexpr CMT_INLINE vec<T, N> copysign(const vec<T, N>& x, const vec<T, N>& y)
{
    return (x & internal::highbitmask<T>) | (y & internal::highbitmask<T>);
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> isnan(const vec<T, N>& x)
{
    return x != x;
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> isinf(const vec<T, N>& x)
{
    return x == c_infinity<T> || x == -c_infinity<T>;
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> isfinite(const vec<T, N>& x)
{
    return !isnan(x) && !isinf(x);
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> isnegative(const vec<T, N>& x)
{
    return (x & internal::highbitmask<T>) != 0;
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> ispositive(const vec<T, N>& x)
{
    return !isnegative(x);
}

template <typename T, size_t N>
CMT_INLINE mask<T, N> iszero(const vec<T, N>& x)
{
    return x == T();
}

/// @brief Swap byte order
template <typename T, size_t N, KFR_ENABLE_IF(sizeof(vec<T, N>) > 8)>
CMT_INLINE vec<T, N> swapbyteorder(const vec<T, N>& x)
{
    return bitcast<T>(swap<sizeof(T)>(bitcast<u8>(x)));
}
template <typename T, KFR_ENABLE_IF(sizeof(T) == 8)>
CMT_INLINE T swapbyteorder(const T& x)
{
    return reinterpret_cast<const T&>(__builtin_bswap64(reinterpret_cast<const u64&>(x)));
}
template <typename T, KFR_ENABLE_IF(sizeof(T) == 4)>
CMT_INLINE T swapbyteorder(const T& x)
{
    return reinterpret_cast<const T&>(__builtin_bswap32(reinterpret_cast<const u32&>(x)));
}
template <typename T, KFR_ENABLE_IF(sizeof(T) == 2)>
CMT_INLINE T swapbyteorder(const T& x)
{
    return reinterpret_cast<const T&>(__builtin_bswap16(reinterpret_cast<const u16&>(x)));
}
KFR_FN(swapbyteorder)

/// @brief Sum all elements of the vector
template <typename T, size_t N>
CMT_INLINE T hadd(const vec<T, N>& value)
{
    return horizontal(value, fn::add());
}
KFR_FN(hadd)

/// @brief Multiply all elements of the vector
template <typename T, size_t N>
CMT_INLINE T hmul(const vec<T, N>& value)
{
    return horizontal(value, fn::mul());
}
KFR_FN(hmul)

template <typename T, size_t N>
CMT_INLINE T hbitwiseand(const vec<T, N>& value)
{
    return horizontal(value, fn::bitwiseand());
}
KFR_FN(hbitwiseand)
template <typename T, size_t N>
CMT_INLINE T hbitwiseor(const vec<T, N>& value)
{
    return horizontal(value, fn::bitwiseor());
}
KFR_FN(hbitwiseor)
template <typename T, size_t N>
CMT_INLINE T hbitwisexor(const vec<T, N>& value)
{
    return horizontal(value, fn::bitwisexor());
}
KFR_FN(hbitwisexor)

/// @brief Calculate the Dot-Product of two vectors
template <typename T, size_t N>
CMT_INLINE T dot(const vec<T, N>& x, const vec<T, N>& y)
{
    return hadd(x * y);
}
KFR_FN(dot)

/// @brief Calculate the Arithmetic mean of all elements in the vector
template <typename T, size_t N>
CMT_INLINE T avg(const vec<T, N>& value)
{
    return hadd(value) / N;
}
KFR_FN(avg)

/// @brief Calculate the RMS of all elements in the vector
template <typename T, size_t N>
CMT_INLINE T rms(const vec<T, N>& value)
{
    return internal::builtin_sqrt(hadd(value * value) / N);
}
KFR_FN(rms)

template <typename T, size_t N, KFR_ENABLE_IF(N >= 2)>
CMT_INLINE vec<T, N> subadd(const vec<T, N>& a, const vec<T, N>& b)
{
    return blend<1, 0>(a + b, a - b);
}
template <typename T, size_t N, KFR_ENABLE_IF(N >= 2)>
CMT_INLINE vec<T, N> addsub(const vec<T, N>& a, const vec<T, N>& b)
{
    return blend<0, 1>(a + b, a - b);
}
KFR_FN(subadd)
KFR_FN(addsub)

template <typename T, size_t N>
CMT_INLINE vec<T, N> negeven(const vec<T, N>& x)
{
    return x ^ broadcast<N>(-T(), T());
}
template <typename T, size_t N>
CMT_INLINE vec<T, N> negodd(const vec<T, N>& x)
{
    return x ^ broadcast<N>(T(), -T());
}

#define KFR_EXPR_UNARY(fn, op)                                                                               \
    template <typename A1, KFR_ENABLE_IF(is_input_expression<A1>::value)>                                    \
    CMT_INLINE auto operator op(A1&& a1)->decltype(bind_expression(fn(), std::forward<A1>(a1)))              \
    {                                                                                                        \
        return bind_expression(fn(), std::forward<A1>(a1));                                                  \
    }

#define KFR_EXPR_BINARY(fn, op)                                                                              \
    template <typename A1, typename A2, KFR_ENABLE_IF(is_input_expressions<A1, A2>::value)>                  \
    CMT_INLINE auto operator op(A1&& a1, A2&& a2)                                                            \
        ->decltype(bind_expression(fn(), std::forward<A1>(a1), std::forward<A2>(a2)))                        \
    {                                                                                                        \
        return bind_expression(fn(), std::forward<A1>(a1), std::forward<A2>(a2));                            \
    }

KFR_EXPR_UNARY(fn::neg, -)
KFR_EXPR_UNARY(fn::bitwisenot, ~)

KFR_EXPR_BINARY(fn::add, +)
KFR_EXPR_BINARY(fn::sub, -)
KFR_EXPR_BINARY(fn::mul, *)
KFR_EXPR_BINARY(fn::div, /)
KFR_EXPR_BINARY(fn::bitwiseand, &)
KFR_EXPR_BINARY(fn::bitwiseor, |)
KFR_EXPR_BINARY(fn::bitwisexor, ^)
KFR_EXPR_BINARY(fn::shl, <<)
KFR_EXPR_BINARY(fn::shr, >>)

KFR_EXPR_BINARY(fn::equal, ==)
KFR_EXPR_BINARY(fn::notequal, !=)
KFR_EXPR_BINARY(fn::less, <)
KFR_EXPR_BINARY(fn::greater, >)
KFR_EXPR_BINARY(fn::lessorequal, <=)
KFR_EXPR_BINARY(fn::greaterorequal, >=)
#undef KFR_EXPR_UNARY
#undef KFR_EXPR_BINARY

template <typename T, size_t N1, size_t... Ns>
vec<vec<T, sizeof...(Ns) + 1>, N1> packtranspose(const vec<T, N1>& x, const vec<T, Ns>&... rest)
{
    const vec<T, N1*(sizeof...(Ns) + 1)> t = transpose<N1>(concat(x, rest...));
    return compcast<vec<T, sizeof...(Ns) + 1>>(t);
}

KFR_FN(packtranspose)
}
