/*	Types.h
	Copyright 2008-2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef TYPES_H
#define TYPES_H

#ifndef _MSC_VER
#include <cstdint>
#else
typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;
#endif

typedef uint32_t EntityId;
typedef	uint8_t PlayerId;
typedef uint8_t	MessageType;
typedef	uint8_t InputId;

const double kPi_d = 3.14159265358979323846;
const float kPi_f = 3.14159265358979323846f;
const double kPi = kPi_d;

#endif /* TYPES_H */
