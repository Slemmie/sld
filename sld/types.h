// ----------------------
//  contains basic types
// ----------------------

#pragma once

#ifndef _SLD_TYPES_H
#define _SLD_TYPES_H

#include <sld/core.h>

_SLD_BEGIN

typedef unsigned char       uint8;
typedef signed   char        int8;

typedef unsigned short     uint16;
typedef signed   short      int16;

typedef unsigned int       uint32;
typedef signed   int        int32;

typedef unsigned long long uint64;
typedef signed   long long  int64;

typedef float         float4;
typedef double        float8;
typedef long double  float12;

typedef float        double4;
typedef double       double8;
typedef long double double12;

typedef uint8        byte;
typedef  int8 signed_byte;

_SLD_END

#endif /// _SLD_TYPES_H
