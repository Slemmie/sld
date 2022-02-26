// ----------------------------
//  contains utility functions
// ----------------------------

#pragma once

#ifndef _SLD_UTILITY_H
#define _SLD_UTILITY_H

#include <sld/core.h>

_SLD_BEGIN

template <typename A> inline A&& move(A& _value) noexcept {
	return static_cast <A&&> (_value);
}

template <typename A> inline void swap(A& _value1, A& _value2) {
	A temp = _SLD move(_value1);
	_value1 = _SLD move(_value2);
	_value2 = _SLD move(temp);
}

_SLD_END

#endif /// _SLD_UTILITY_H
