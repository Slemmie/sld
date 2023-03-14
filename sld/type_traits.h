#pragma once

#ifndef _SLD_TYPE_TRAITS_H
#define _SLD_TYPE_TRAITS_H

#include "core.h"

_SLD_BEGIN

template <typename _TY> struct remove_reference         { typedef _TY type; };
template <typename _TY> struct remove_reference <_TY&>  { typedef _TY type; };
template <typename _TY> struct remove_reference <_TY&&> { typedef _TY type; };

template <typename _TY> using remove_reference_t = typename remove_reference <_TY>::type;

template <bool, typename _TY = void> struct enable_if             { };
template <typename _TY>              struct enable_if <true, _TY> { typedef _TY type; };
template <bool _CONDITION, typename _TY = void> using enable_if_t = typename enable_if <_CONDITION, _TY>::type;

template <typename _TY, _TY _VAL> struct integral_constant {
	static constexpr _TY value = _VAL;

	typedef _TY value_type;

	typedef integral_constant <_TY, _VAL> type;

	constexpr operator value_type ()    const noexcept { return value; }
	constexpr value_type operator () () const noexcept { return value; }
};

using true_type = integral_constant <bool, true>;
using false_type = integral_constant <bool, false>;

template <bool _VAL> using _bool_constant = integral_constant <bool, _VAL>;

// metaprogramming helpers

template <typename _TY, typename... _TYS> using _first_t = _TY;

template <typename... _TYS> auto _or_fn (int) -> _first_t <false_type, enable_if_t <! bool (_TYS::value)> ...>;
template <typename... _TYS> auto _or_fn (...) -> true_type;
template <typename... _TYS> struct _or : decltype(_or_fn <_TYS...> (0)) { };

template <typename... _TYS> auto _and_fn (int) -> _first_t <true_type, enable_if_t <bool (_TYS::value)> ...>;
template <typename... _TYS> auto _and_fn (...) -> false_type;
template <typename... _TYS> struct _and : decltype(_and_fn <_TYS...> (0)) { };

template <typename _TY> struct _not : public _bool_constant <! bool (_TY::value)> { };

// add lval/rval reference

template <typename _TY> struct _add_lval_ref { typedef _TY& type; };
template <typename _TY> using _add_lval_ref_t = typename _add_lval_ref <_TY>::type;

template <typename _TY> struct _add_rval_ref { typedef _TY&& type; };
template <typename _TY> using _add_rval_ref_t = typename _add_rval_ref <_TY>::type;

// is constructible

template <typename _TY, typename... _ARGS> struct _is_constructible_impl : public _bool_constant <__is_constructible(_TY, _ARGS...)> { };

template <typename _TY, typename... _ARGS> struct is_constructible      : public _is_constructible_impl <_TY, _ARGS...>                    { };
template <typename _TY>                    struct is_copy_constructible : public _is_constructible_impl <_TY, _add_lval_ref_t <const _TY>> { };
template <typename _TY>                    struct is_move_constructible : public _is_constructible_impl <_TY, _add_rval_ref_t <_TY>>       { };

// is assignable

template <typename _TY1, typename _TY2> struct _is_assignable_impl : public _bool_constant <__is_assignable(_TY1, _TY2)> { };

template <typename _TY1, typename _TY2> struct is_assignable      : public _is_assignable_impl <_TY1, _TY2>                                         { };
template <typename _TY>                 struct is_copy_assignable : public _is_assignable_impl <_add_lval_ref_t <_TY>, _add_lval_ref_t <const _TY>> { };
template <typename _TY>                 struct is_move_assignable : public _is_assignable_impl <_add_lval_ref_t <_TY>, _add_rval_ref_t <_TY>>       { };

// is nothrow constructible

template <typename _TY, typename... _ARGS> struct _is_nothrow_constructible_impl : public _bool_constant <__is_nothrow_constructible(_TY, _ARGS...)> { };

template <typename _TY, typename... _ARGS> struct is_nothrow_constructible      : public _is_nothrow_constructible_impl <_TY, _ARGS...>                    { };
template <typename _TY>                    struct is_nothrow_copy_constructible : public _is_nothrow_constructible_impl <_TY, _add_lval_ref_t <const _TY>> { };
template <typename _TY>                    struct is_nothrow_move_constructible : public _is_nothrow_constructible_impl <_TY, _add_rval_ref_t <_TY>>       { };

// is nothrow assignable

template <typename _TY1, typename _TY2> struct _is_nothrow_assignable_impl : public _bool_constant <__is_nothrow_assignable(_TY1, _TY2)> { };

template <typename _TY1, typename _TY2> struct is_nothrow_assignable      : public _is_assignable_impl <_TY1, _TY2>                                         { };
template <typename _TY>                 struct is_nothrow_copy_assignable : public _is_assignable_impl <_add_lval_ref_t <_TY>, _add_lval_ref_t <const _TY>> { };
template <typename _TY>                 struct is_nothrow_move_assignable : public _is_assignable_impl <_add_lval_ref_t <_TY>, _add_rval_ref_t <_TY>>       { };

_SLD_END

#endif // _SLD_TYPE_TRAITS_H
