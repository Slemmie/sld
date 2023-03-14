#pragma once

#ifndef _SLD_UTILITY_H
#define _SLD_UTILITY_H

#include "core.h"
#include "type_traits.h"

_SLD_BEGIN

template <typename _TY> constexpr remove_reference_t <_TY> && move(_TY&& _arg) noexcept {
	return static_cast <remove_reference_t <_TY> &&> (_arg);
}

template <typename _TY> constexpr
enable_if_t <_and <is_move_constructible <_TY>, is_move_assignable <_TY>>::value>
swap(_TY& _arg1, _TY& _arg2) noexcept(_and <is_nothrow_move_constructible <_TY>, is_nothrow_move_assignable <_TY>>::value) {
	_TY _temp = move(_arg1);
	_arg1 = move(_arg2);
	_arg2 = move(_temp);
}

_SLD_END

#endif // _SLD_UTILITY_H
