// ----------------------------------
//  defines various basic algorithms
// ----------------------------------

#pragma once

#ifndef _SLD_ALGORITHM_H
#define _SLD_ALGORITHM_H

#include <sld/core.h>

_SLD_BEGIN

// begin min/max functions
// custom comparators should return whether arg1 < arg2, regardless of use of min()/max()

template <class A> inline const A& min(const A& _v1, const A& _v2) {
	return _v2 < _v1 ? _v2 : _v1;
}

template <class A, class CMP> inline const A& min(const A& _v1, const A& _v2, const CMP& _cmp) {
	return _cmp(_v2, _v1) ? _v2 : _v1;
}

template <class A> inline const A& max(const A& _v1, const A& _v2) {
	return _v1 < _v2 ? _v2 : _v1;
}

template <class A, class CMP> inline const A& max(const A& _v1, const A& _v2, const CMP& _cmp) {
	return cmp(_v1, _v2) ? _v2 : _v1;
}

// end min/max functions

// returns absolute value of arg, required < operator and - operator
template <class A> inline const A& abs(const A& _v1) {
	return _SLD max(_v1, -_v1);
}

_SLD_END

#endif /// _SLD_ALGORITHM_H
