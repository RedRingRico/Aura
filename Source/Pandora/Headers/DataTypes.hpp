#ifndef __AURA_DATATYPES_HPP__
#define __AURA_DATATYPES_HPP__

#include <stddef.h>
#include <stdint.h>

typedef signed char		AUR_SCHAR;
typedef unsigned char	AUR_UCHAR;
typedef int8_t			AUR_SINT8;
typedef uint8_t			AUR_UINT8;
typedef int16_t			AUR_SINT16;
typedef uint16_t		AUR_UINT16;
typedef int32_t			AUR_SINT32;
typedef uint32_t		AUR_UINT32;
typedef int64_t			AUR_SINT64;
typedef uint64_t		AUR_UINT64;
typedef float			AUR_FLOAT32;
typedef double			AUR_FLOAT64;
typedef size_t			AUR_MEMSIZE;

typedef AUR_UCHAR	AUR_BYTE;
typedef AUR_UINT32	AUR_BOOL;

const AUR_UINT32 AUR_TRUE	= 1;
const AUR_UINT32 AUR_FALSE	= 0;

const AUR_UINT32 AUR_OK		= 0x00000000;
const AUR_UINT32 AUR_FAIL	= 0xFFFFFFFF;

#define AUR_NULL nullptr

#endif // __AURA_DATATYPES_HPP__

