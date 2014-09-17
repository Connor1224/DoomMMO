/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_Types_H
#define Navigation_Types_H


#include "gwnavruntime/kernel/SF_Types.h"

#if defined(KY_OS_XBOX360)
# include <ppcintrinsics.h>
#endif

#if defined(KY_OS_PS3)
# include <ppu_intrinsics.h>
#else
# include <math.h>
#endif

//------------------------------------------------------------------------------------------------
// WIN32/64 (KY_OS_WIN32 means 32bits and 64bits)
#if defined (KY_OS_WIN32)

typedef int              KyInt32;   ///< Type used internally to represent a 32-bit integer.
typedef unsigned int     KyUInt32;  ///< Type used internally to represent an unsigned 32-bit integer.
typedef __int64          KyInt64;   ///< Type used internally to represent a 64-bit integer.
typedef unsigned __int64 KyUInt64;  ///< Type used internally to represent an unsigned 64-bit integer.
typedef short            KyInt16;   ///< Type used internally to represent a 16-bit integer.
typedef unsigned short   KyUInt16;  ///< Type used internally to represent an unsigned 16-bit integer.
typedef unsigned char    KyUInt8;   ///< Type used internally to represent an unsigned 8-bit integer.
typedef signed char      KyInt8;    ///< Type used internally to represent an 8-bit integer.
typedef float            KyFloat32; ///< Type used internally to represent a 32-bit floating-point number.
typedef double           KyFloat64; ///< Type used internally to represent a 64-bit floating-point number.

#endif


//------------------------------------------------------------------------------------------------
// XBOX360
#if defined(KY_OS_XBOX360)

typedef int              KyInt32;
typedef unsigned int     KyUInt32;
typedef __int64          KyInt64;
typedef unsigned __int64 KyUInt64;
typedef short            KyInt16;
typedef unsigned short   KyUInt16;
typedef unsigned char    KyUInt8;
typedef signed char      KyInt8;
typedef float            KyFloat32;
typedef double           KyFloat64;

#endif

//------------------------------------------------------------------------------------------------
// WII
#if defined (KY_OS_WIIU)

typedef int                KyInt32;
typedef unsigned int       KyUInt32;
typedef long long          KyInt64;
typedef unsigned long long KyUInt64;
typedef short              KyInt16;
typedef unsigned short     KyUInt16;
typedef unsigned char      KyUInt8;
typedef signed char        KyInt8;
typedef float              KyFloat32;
typedef double             KyFloat64;

#endif

//-----------------------------------------------------------------------------------------------
// LINUX64
#if defined(KY_OS_LINUX) && defined(KY_64BIT_POINTERS)

typedef int               KyInt32;
typedef unsigned int      KyUInt32;
typedef long              KyInt64;
typedef unsigned long     KyUInt64;
typedef short             KyInt16;
typedef unsigned short    KyUInt16;
typedef unsigned char     KyUInt8;
typedef signed char       KyInt8;
typedef float             KyFloat32;
typedef double            KyFloat64;

#endif

//-----------------------------------------------------------------------------------------------
// LINUX32

#if defined(KY_OS_LINUX) && !defined(KY_64BIT_POINTERS)

typedef int                KyInt32;
typedef unsigned int       KyUInt32;
typedef long long          KyInt64;
typedef unsigned long long KyUInt64;
typedef short              KyInt16;
typedef unsigned short     KyUInt16;
typedef unsigned char      KyUInt8;
typedef signed char        KyInt8;
typedef float              KyFloat32;
typedef double             KyFloat64;

#endif

//-----------------------------------------------------------------------------------------------
// MAC64
#if defined(KY_OS_MAC) && defined(KY_64BIT_POINTERS)

typedef int              KyInt32;
typedef unsigned int     KyUInt32;
typedef long             KyInt64;
typedef unsigned long    KyUInt64;
typedef short            KyInt16;
typedef unsigned short   KyUInt16;
typedef unsigned char    KyUInt8;
typedef signed char      KyInt8;
typedef float            KyFloat32;
typedef double           KyFloat64;

#endif

//-----------------------------------------------------------------------------------------------
// MAC32
#if defined (KY_OS_MAC) && !defined(KY_64BIT_POINTERS)

typedef int                KyInt32;
typedef unsigned int       KyUInt32;
typedef long long          KyInt64;
typedef unsigned long long KyUInt64;
typedef short              KyInt16;
typedef unsigned short     KyUInt16;
typedef unsigned char      KyUInt8;
typedef signed char        KyInt8;
typedef float              KyFloat32;
typedef double             KyFloat64;

#endif


//-----------------------------------------------------------------------------------------------
// NGP
#if defined (KY_OS_PSVITA)

typedef int                 KyInt32;
typedef unsigned int        KyUInt32;
typedef long long           KyInt64;
typedef unsigned long long  KyUInt64;
typedef short               KyInt16;
typedef unsigned short      KyUInt16;
typedef unsigned char       KyUInt8;
typedef signed char         KyInt8;
typedef float               KyFloat32;
typedef double              KyFloat64;

#endif


//-----------------------------------------------------------------------------------------------
// PS3
#if defined (KY_OS_PS3)

typedef int                KyInt32;
typedef unsigned int       KyUInt32;
typedef long long          KyInt64;
typedef unsigned long long KyUInt64;
typedef short              KyInt16;
typedef unsigned short     KyUInt16;
typedef unsigned char      KyUInt8;
typedef signed char        KyInt8;
typedef float              KyFloat32;
typedef double             KyFloat64;

#endif

//-----------------------------------------------------------------------------------------------
// ORBIS
#if defined (KY_OS_ORBIS)

typedef int                KyInt32;
typedef unsigned int       KyUInt32;
typedef long long          KyInt64;
typedef unsigned long long KyUInt64;
typedef short              KyInt16;
typedef unsigned short     KyUInt16;
typedef unsigned char      KyUInt8;
typedef signed char        KyInt8;
typedef float              KyFloat32;
typedef double             KyFloat64;

#endif


//----------------------------------------------------------------------------------------
// types MAXVAL and MINVAL
#define KyInt32MAXVAL   0x7FFFFFFF ///< The maximum value that can be stored in the #KyInt32 variable type.
#define KyInt32MINVAL   0x80000000 ///< The minimum value that can be stored in the #KyInt32 variable type.
#define KyUInt32MAXVAL  0xFFFFFFFF ///< The maximum value that can be stored in the #KyUInt32 variable type.
#define KyFloat32MAXVAL 3.402823466e+38f ///< The maximum value that can be stored in the #KyFloat32 variable type.
#define KyInt16MAXVAL   0x7FFF ///< The maximum value that can be stored in the #KyInt16 variable type.
#define KyInt16MINVAL   0x8000 ///< The minimum value that can be stored in the #KyInt16 variable type.
#define KyUInt16MAXVAL  0xFFFF ///< The maximum value that can be stored in the #KyUInt16 variable type.
#define KyInt8MAXVAL    0x7F   ///< The maximum value that can be stored in the #KyInt8 variable type.
#define KyInt8MINVAL    0x80   ///< The maximum value that can be stored in the #KyInt8 variable type.
#define KyUInt8MAXVAL   0xFF   ///< The maximum value that can be stored in the #KyUInt8 variable type.
#define KyInt64MAXVAL   0x7FFFFFFFFFFFFFFFLL  ///< The maximum value that can be stored in the #KyInt64 variable type.
#define KyUInt64MAXVAL  0xFFFFFFFFFFFFFFFFULL ///< The maximum value that can be stored in the #KyUInt64 variable type.


//----------------------------------------------------------------------------------------
// KY_PERF_MARKERS_ENABLED
#if defined(KY_BUILD_RELEASE)
# define KY_PERF_MARKERS_ENABLED
#endif


//----------------------------------------------------------------------------------------
// KY_NULL
#define KY_NULL 0


//----------------------------------------------------------------------------------------
// KyResult, KY_ERROR, KY_SUCCESS, KY_FAILED(), KY_SUCCEEDED()

/// Defines a type that can be returned by methods or functions in the Gameware Navigation SDK to indicate the results of the requested computations.
typedef KyInt32 KyResult;

namespace Kaim
{
namespace Result
{
	static const KyResult Success =  1; ///< Indicates that the requested operation was successfully completed.
	static const KyResult Failure =  0; ///< Indicates that the requested operation could not be successfully completed. This return indicates the failure of a "Try" request.

	/// Returns true if the specified result code indicates that the requested operation
	/// successfully completed.
	inline bool Check(KyResult result) { return result == Success; }

	/// Returns true if the specified result code indicates that the requested operation failed.
	inline bool Fail(KyResult result) { return result == Failure; }
}
}

#define KY_ERROR   Kaim::Result::Failure ///< Shorthand for Kaim::Result::Failure.
#define KY_SUCCESS Kaim::Result::Success ///< Shorthand for Kaim::Result::Success.
#define KY_FAILED(expression)    (Kaim::Result::Fail(expression))  ///< Shorthand for Kaim::Result::Fail().
#define KY_SUCCEEDED(expression) (Kaim::Result::Check(expression)) ///< Shorthand for Kaim::Result::Check().

// Returns KY_ERROR if expression == KY_ERROR
#define KY_FORWARD_ERROR_NO_LOG(expression) { KyResult result = (expression); if (result == KY_ERROR) { return result; }}


//----------------------------------------------------------------------------------------
// KY_TEMPORARY_CODE
#ifndef KY_TEMPORARY_CODE
# define KY_TEMPORARY_CODE KY_COMPILE_TIME_MESSAGE("[KY_TEMPORARY_CODE] ########### !! TEMPORARY CODE TO BE REMOVED !! ###########")
#endif


//----------------------------------------------------------------------------------------
// KY_DEPRECATED
/// Macro to mark a function, class or method as deprecated.
/// The compiler issues a warning when a deprecated element is used.
#ifndef KY_DEPRECATED
# if (defined (KY_OS_WIN32) || defined (KY_OS_XBOX360)) && !defined (KY_CC_GNU)
#  define KY_DEPRECATED(f) __declspec(deprecated) f
# elif defined(KY_OS_LINUX) || defined(KY_OS_MAC) || defined (KY_OS_PS3) || defined (KY_CC_GNU)
#  define KY_DEPRECATED(f) f __attribute__ ((deprecated))
# else
#  define KY_DEPRECATED(f) f
# endif
#endif

#if 0 // Following shows how to use macro KY_DEPRECATED
// ------------ Correct way ------------
void func1(int) {}
KY_DEPRECATED (void func1(int));
struct A
{
	KY_DEPRECATED(void f()) {}
};
KY_DEPRECATED(struct) B {};
// ------------ Incorrect way ------------
// KY_DEPRECATED (void func2(int)) {} // Doesn't work with gcc (function-definition)
class C
{
	void f() {}
};
// KY_DEPRECATED(void C::f()); // Doesn't work with msvc (error C2761: member function redeclaration not allowed)
// KY_DEPRECATED(struct D) {}; // Doesn't work with gcc
// struct KY_DEPRECATED(E) {}; // Doesn't work with gcc
#endif


//----------------------------------------------------------------------------------------
// KY_MACRO_START KY_MACRO_END
#if defined(KY_CC_MSVC)
# define KY_MACRO_START                \
	do {                               \
	KY_PRAGMA(warning(push))           \
	KY_PRAGMA(warning(disable:4127))
# define KY_MACRO_END \
	} while(0)        \
	KY_PRAGMA(warning(pop))
#else
# define KY_MACRO_START do {
# define KY_MACRO_END } while(0)
#endif


//----------------------------------------------------------------------------------------
// KY_DISABLE_ALIGN_WARNING_START KY_DISABLE_ALIGN_WARNING_END
#if defined(KY_CC_MSVC)
# define KY_DISABLE_ALIGN_WARNING_START KY_PRAGMA(warning (push)) KY_PRAGMA(warning (disable:4324)) // structure was padded due to __declspec(align())
# define KY_DISABLE_ALIGN_WARNING_END KY_PRAGMA(warning (pop))
#else
# define KY_DISABLE_ALIGN_WARNING_START
# define KY_DISABLE_ALIGN_WARNING_END
#endif


//----------------------------------------------------------------------------------------
// KY_FORCE_ALIGNMENT
#if (defined (KY_OS_WIN32) || defined (KY_OS_XBOX360)) && !defined (KY_CC_GNU)
# define KY_FORCE_ALIGNMENT(alignment, declaration) __declspec(align(alignment)) declaration
#elif defined(KY_OS_LINUX) || defined(KY_OS_MAC) || defined (KY_OS_PS3) || defined (KY_OS_WII) || defined (KY_CC_GNU) || defined (KY_OS_PSVITA)
# define KY_FORCE_ALIGNMENT(alignment, declaration) declaration __attribute__((aligned(alignment)))
#endif


//----------------------------------------------------------------------------------------
// ArraySize
namespace Kaim
{
/// Returns the size of a fixed-size array.
template<typename T, KyUInt32 N>
KY_INLINE KyUInt32 ArraySize(T (&) [N]) { return N; }
}

//----------------------------------------------------------------------------------------
// trigonometry
namespace Kaim
{

/// Stores the value of pi.
static const KyFloat32 KY_PI = 3.14159265f;

/// Stores the value of #KY_PI divided by 180.
const KyFloat32 KY_PI_DIVIDED_BY_180 = KY_PI / 180.0f;

/// Returns angle in radians converted from angle in degrees
inline KyFloat32 GetRadiansFromDegrees(KyFloat32 degrees) { return degrees * KY_PI_DIVIDED_BY_180; }

}


//----------------------------------------------------------------------------------------
// file path
namespace Kaim
{

enum FilePath
{
#if defined (KY_OS_XBOX360) || defined(KY_OS_WIN32)
	FilePath_Separator = '\\',
	FilePath_OppositeSeparator = '/'
#else
	FilePath_Separator = '/',
	FilePath_OppositeSeparator = '\\'
#endif
};

}



//----------------------------------------------------------------------------------------
// KY_CLASS_WITHOUT_COPY
/// Define to forbid copy constructor and copy assignment. 
#define KY_CLASS_WITHOUT_COPY(ClassName) \
private:                                 \
	ClassName(const ClassName& rhs);     \
	ClassName& operator=(const ClassName& rhs); 


#endif // Navigation_Types_H
