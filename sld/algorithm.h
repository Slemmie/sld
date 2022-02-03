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

template <class A> inline const A& min(const A& v1, const A& v2) {
	return v2 < v1 ? v2 : v1;
}

template <class A, class CMP> inline const A& min(const A& v1, const A& v2, const CMP& cmp) {
	return cmp(v2, v1) ? v2 : v1;
}

template <class A> inline const A& max(const A& v1, const A& v2) {
	return v1 < v2 ? v2 : v1;
}

template <class A, class CMP> inline const A& max(const A& v1, const A& v2, const CMP& cmp) {
	return cmp(v1, v2) ? v2 : v1;
}

// end min/max functions

_SLD_END

#endif /// _SLD_ALGORITHM_H
