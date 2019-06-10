#pragma once

#ifdef _MSC_VER
#else
#include "pal/pal.h"
#define constexpr
#define nullptr 0
#define sprintf_s snprintf
#define _stricmp strcasecmp
#ifndef FLT_MAX
#define FLT_MAX		((float)3.40282346638528860e+38)
#endif
#endif


// Unsigned base types.
typedef unsigned char 		uint8;		// 8-bit  unsigned.
typedef unsigned short int	uint16;		// 16-bit unsigned.
typedef unsigned int		uint32;		// 32-bit unsigned.
typedef unsigned long long	uint64;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			int8;		// 8-bit  signed.
typedef signed short int	int16;		// 16-bit signed.
typedef signed int	 		int32;		// 32-bit signed.
#ifdef _MSC_VER
typedef signed long long	int64;		// 64-bit signed.
#else
typedef int64_t	int64;		// 64-bit signed.
#endif

// Character types.
typedef char				ANSICHAR;	// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
typedef wchar_t				WIDECHAR;	// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
typedef uint8				CHAR8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint16				CHAR16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint32				CHAR32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
#ifdef _MSC_VER
typedef WIDECHAR			TCHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.
#else
#endif

// typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::TIntPointer UPTRINT;	// unsigned int the same size as a pointer
// typedef SelectIntPointerType<int32, int64, sizeof(void*)>::TIntPointer PTRINT;		// signed int the same size as a pointer
// typedef UPTRINT SIZE_T;																// unsigned int the same size as a pointer
// typedef PTRINT SSIZE_T;																// signed int the same size as a pointer

typedef int32					TYPE_OF_NULL;
//typedef decltype(nullptr)		TYPE_OF_NULLPTR;


/** Type of tick we wish to perform on the level */
enum ELevelTick
{
	/** Update the level time only. */
	LEVELTICK_TimeOnly = 0,
	/** Update time and viewports. */
	LEVELTICK_ViewportsOnly = 1,
	/** Update all. */
	LEVELTICK_All = 2,
	/** Delta time is zero, we are paused. Components don't tick. */
	LEVELTICK_PauseTick = 3,
};