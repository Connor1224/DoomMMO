/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

/**************************************************************************

PublicHeader:   Kernel
Filename    :   KY_Math.h
Content     :   Math inline functions and macro definitions
Created     :   July 7, 2005
Authors     :   Michael Antonov, Maxim Shemanarev

**************************************************************************/

#ifndef INC_KY_Kernel_Math_H
#define INC_KY_Kernel_Math_H

#include "gwnavruntime/kernel/SF_Types.h"

#ifdef KY_MATH_H
#include KY_MATH_H
#elif defined(KY_OS_WINCE)
#include <cmath>
#elif defined(KY_CC_RENESAS)
#include <math.h>
#include <mathf.h>
#else
#include <math.h>
#include <stdlib.h>
#endif


// *** Useful math constants

#define KY_MATH_EPSILON                ((Double)1.0e-12)                   // epsilon 
#define KY_MATH_E                      ((Double)2.7182818284590452354)     // e 
#define KY_MATH_LOG2E                  ((Double)1.4426950408889634074)     // log_2 e 
#define KY_MATH_LOG10E                 ((Double)0.43429448190325182765)    // log_10 e 
#define KY_MATH_LN2                    ((Double)0.69314718055994530942)    // log_e 2 
#define KY_MATH_LN10                   ((Double)2.30258509299404568402)    // log_e 10 
#define KY_MATH_PI                     ((Double)3.14159265358979323846)    // pi 
#define KY_MATH_PI_2                   ((Double)1.57079632679489661923)    // pi/2 
#define KY_MATH_PI_3                   ((Double)1.04719755119659774615)    // pi/3 
#define KY_MATH_PI_4                   ((Double)0.78539816339744830962)    // pi/4 
#define KY_MATH_1_PI                   ((Double)0.31830988618379067154)    // 1/pi 
#define KY_MATH_2_PI                   ((Double)0.63661977236758134308)    // 2/pi 
#define KY_MATH_2_SQRTPI               ((Double)1.12837916709551257390)    // 2/sqrt(pi) 
#define KY_MATH_SQRT2                  ((Double)1.41421356237309504880)    // sqrt(2) 
#define KY_MATH_SQRT1_2                ((Double)0.70710678118654752440)    // 1/sqrt(2) 
#define KY_MATH_DEGTORAD               ((Double)0.01745329251994329577)    // pi/180 
#define KY_MATH_TAN_PI_8               ((Double)0.41421356237309504880)    // tan(pi/8)
#define KY_MATH_SIN_PI_4               ((Double)0.70710678118654752440)    // sin(pi/4)

// Degrees to Radians
#define KY_DEGTORAD(d)                 ((d)*(KY_MATH_PI)/((Double)180.0))
#define KY_RADTODEG(r)                 ((r)*((Double)180.0)/(KY_MATH_PI))

#endif // INC_KY_Kernel_Math_H
