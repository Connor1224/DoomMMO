/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/
// primary contact: LASI - secondary contact: NOBODY

#include <assert.h>
#include "gwnavruntime/math/vec2i.h"

namespace Kaim
{
namespace IntegerSegment
{

// Generic template functions

// returns true iff pt coordinate is between start and end
template <typename T>
static KY_INLINE bool Between(const T& pt, const T& edgeStart, const T& edgeEnd)
{
	return pt < edgeEnd && edgeStart < pt;
}

// returns true if [ edgeStart, edgeEnd ] is vertical
template <typename T>
static KY_INLINE bool IsVertical(const T& edgeStart, const T& edgeEnd)
{
	return edgeStart.x == edgeEnd.x;
}
	
// returns true if [ edgeStart, edgeEnd ] is vertical
template <typename T>
static KY_INLINE bool IsHorizontal(const T& edgeStart, const T& edgeEnd)
{
	return edgeStart.y == edgeEnd.y;
}

template <typename T> //T E { Int32/64 } 
static bool LessSlopeUnit(T dx1, T dy1, T dx2, T dy2)
{
	//reflect x and y slopes to right hand side half plane
	if(dx1 < 0) { dy1 *= -1; dx1 *= -1; } 
	else if(dx1 == 0) 	
	{
		//if the first slope is vertical the first cannot be less
		return false;
	}
	if(dx2 < 0) {dy2 *= -1;	dx2 *= -1; }  
	else if(dx2 == 0) 
	{
		//if the second slope is vertical the first is always less unless it is also vertical, in which case they are equal 
		return dx1 != 0;
	}
	T cross_1 = dx2 * (dy1 < 0 ? -dy1 : dy1);
	T cross_2 = dx1 * (dy2 < 0 ? -dy2 : dy2);
	T dx1_sign = 1;
	T dx2_sign = 1;
	T dy1_sign = dy1 < 0 ? -1 : 1;
	T dy2_sign = dy2 < 0 ? -1 : 1;
	T cross_1_sign = dx2_sign * dy1_sign;
	T cross_2_sign = dx1_sign * dy2_sign;
	if(cross_1_sign < cross_2_sign) return true;
	if(cross_2_sign < cross_1_sign) return false;
	if(cross_1_sign == -1) return cross_2 < cross_1;
	return cross_1 < cross_2;
}

template <typename T>  //T E { Int32 / Int64 }
static KY_INLINE bool LessSlope(KyInt32 x, KyInt32 y, const Vec2i& pt1, const Vec2i& pt2)
{
	const Vec2i* pts[2] = {&pt1, &pt2};
	//compute y value on edge from pt_ to pts[1] at the x value of pts[0]
	T dy2 = pts[1]->y - y;
	T dy1 = pts[0]->y - y;
	T dx2 = pts[1]->x - x;
	T dx1 = pts[0]->x - x;
	return LessSlopeUnit<T>(dx1, dy1, dx2, dy2);
}

template <typename T> //Int32 / Int64
static KY_INLINE bool EqualSlopes(KyInt32 x, KyInt32 y, const Vec2i& pt1, const Vec2i& pt2)
{
	const Vec2i* pts[2] = {&pt1, &pt2};
	T dy2 = pts[1]->y - y;
	T dy1 = pts[0]->y - y;
	T dx2 = pts[1]->x - x;
	T dx1 = pts[0]->x - x;
	return dx1 * dy2 == dx2 * dy1;
}


//return -1 below, 0 on and 1 above line
template <typename T>  //Int32 / Int64
static KY_INLINE int OnAboveOrBelow(const Vec2i& pt, const Vec2i& edgeStart, const Vec2i& edgeEnd)
{
	if(pt == edgeStart || pt == edgeEnd) return 0;
	if(EqualSlopes<T>(pt.x, pt.y, edgeStart, edgeEnd)) return 0;
	bool less_result = LessSlope<T>(pt.x, pt.y, edgeStart, edgeEnd);
	int retval = less_result ? -1 : 1;
	if(edgeEnd < edgeStart) retval *= -1;
	bool between = edgeStart < pt && pt < edgeEnd;
	if(!between) retval *= -1;
	return retval;
}

template <typename T>  //T E { Vec{2,3}i, Vec2LL }
bool SegmentVsSegment2d(const Vec2i& a, const Vec2i& b, const Vec2i& c, const Vec2i& d)
{
	Kaim::Box2i edge1Box, edge2Box;
	edge1Box.ExpandByVec2(a);
	edge1Box.ExpandByVec2(b);

	edge2Box.ExpandByVec2(c);
	edge2Box.ExpandByVec2(d);
/*
	Vec2i edge1 = b-a;
	Vec2i edge2 = d-c;
*/
	if (Kaim::Intersections::AABBVsAABB2d(edge1Box, edge2Box))
	{
		if(a == c) 
		{
			if(b != d && EqualSlopes<T>(a.x, a.y, b, d)) 
			{
				return true;
			} 
			else 
			{
				return false;
			}
		}
		if(a == d) 
		{
			if(b != c && EqualSlopes<T>(a.x, a.y, b, c)) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		if(b == c) 
		{
			if(a != d && EqualSlopes<T>(b.x, b.y, a, d)) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		if(b == d) 
		{
			if(a != c && EqualSlopes<T>(b.x, b.y, a, c)) 
			{
				return true;
			} 
			else 
			{
				return false;
			}
		}
		int oab1 = OnAboveOrBelow<T>(a, c, d);
		if(oab1 == 0 && Between<Kaim::Vec2i>(a, c, d)) return true; 
		int oab2 = OnAboveOrBelow<T>(b, c, d);
		if(oab2 == 0 && Between<Kaim::Vec2i>(b, c, d)) return true; 
		if(oab1 == oab2 && oab1 != 0) return false; // both points of he1 are on same side of he2
		int oab3 = OnAboveOrBelow<T>(c, a, b);
		if(oab3 == 0 && Between<Kaim::Vec2i>(c, a, b)) return true; 
		int oab4 = OnAboveOrBelow<T>(d, a, b);
		if(oab4 == 0 && Between<Kaim::Vec2i>(d, a, b)) return true; 
		if(oab3 == oab4) return false; // both points of he2 are on same side of he1
		return true; // they must cross
	}
	if(IsVertical<Kaim::Vec2i>(a, b) && IsVertical<Kaim::Vec2i>(c, d) && a.x == c.x)
	{
		return edge1Box.Min().y < edge2Box.Max().y && edge1Box.Max().y > edge2Box.Min().y;
	}
	if(IsHorizontal<Kaim::Vec2i>(a, b) && IsHorizontal<Kaim::Vec2i>(c, d) && a.y == c.y)
	{
		return edge1Box.Min().x < edge2Box.Max().x && edge1Box.Max().x > edge2Box.Min().x;
	}
	return false;
}


// TODO: Return INT instead of FLOAT...
// cast from int to float to int = BAD - will LHS on PPC... 
static inline KyFloat32 EvalAtXforY_float(KyInt32 xIn, const Kaim::Vec2i pt, const Kaim::Vec2i other_pt) 
{ 
	KyInt32	/*evalAtXforYret,*/ x1, y1, dx01, dx21, 
		dy21, x2, y2;
	if(pt.y == other_pt.y)
		return (KyFloat32)pt.y;
	x1 = pt.x;
	y1 = pt.y;
	dx01 = xIn - x1;
	if(dx01 == 0) return (KyFloat32)y1;
	x2 = other_pt.x;
	y2 = other_pt.y;

	dx21 = x2 - x1;
	dy21 = y2 - y1;
	return ( ((KyFloat32)dx01 * (KyFloat32)dy21) / (KyFloat32)dx21 + (KyFloat32)y1);
}

 //Returns the Y value of a point at a given X on line [pt;other_pt].
 //Result is rounded to the closest int inferior to the actual position. 
 //eg: 
 //y_float = 1.2f => 1
 //y_float = 0.8f => 0
 //y_float = -0.1f => -1
 //y_float = -0.9f => -1
 //y_float = -1.0001f => -2
 //Computation is carried out in 64 bit and should not overflow. 
 //If input values are not too big (< KyInt16MaxVal), _Int32 method should be preferred. 
static inline KyInt32 EvalAtXforY_Int64(KyInt32 xIn, const Kaim::Vec2i pt, const Kaim::Vec2i other_pt) 
{ 
	KyInt64	x1, y1, dx01, dx21, 
		dy21, x2, y2, num, denum, truncatedResult, remainder;
	if(pt.y == other_pt.y)
		return (KyInt32)pt.y;
	x1 = (KyInt64)pt.x;
	y1 = (KyInt64)pt.y;
	dx01 = xIn - x1;
	if(dx01 == 0) return pt.y;
	x2 = (KyInt64)other_pt.x;
	y2 = (KyInt64)other_pt.y;

	dx21 = x2 - x1;
	dy21 = y2 - y1;

	num = dx01*dy21;
	denum = dx21;
	if (denum == 0) 
	{
		return KyInt32MAXVAL - 1;
	}
	
	truncatedResult = num/denum; 
	// On x86 the remainder is a by-product of the division itself so any half-decent compiler should be able to just use it (and not perform a div again). 
	// This is probably done on other architectures too.
	remainder = num%denum;

	// Determine if result is >0 BEFORE dividing. Otherwise If truncated result is 0, there is no way to know the real sign
	// of the result. 
	KyInt64 sign = num ^ denum; //sign bit will have the correct value with a XOR
	KyInt64 truncationValue = (sign >> 63) & (remainder != 0); // 1 iff sign < 0 && remainder is different from 0. = 0 otherwise
	assert(truncatedResult + y1 <= KyInt32MAXVAL && truncatedResult + y1 > -KyInt32MAXVAL);
	return (KyInt32)(truncatedResult + y1 - truncationValue);
}

//Returns the Y value of a point at a given X on line [pt;other_pt].
//Result is rounded to the closest int from the actual position. 
//eg: 
//y_float = 1.2f => 1
//y_float = 0.8f => 1
//y_float = -0.1f => 0
//y_float = -0.9f => -1
//y_float = -1.0001f => -1
//Computation is carried out in 64 bit and should not overflow. 
static inline KyInt32 EvalAtXforY_Int64_rounded(KyInt32 xIn, const Kaim::Vec2i pt, const Kaim::Vec2i other_pt) 
{ 
	KyInt64	x1, y1, dx01, dx21, 
		dy21, x2, y2, num, denum, roundedResult;
	if(pt.y == other_pt.y)
		return (KyInt32)pt.y;
	x1 = (KyInt64)pt.x;
	y1 = (KyInt64)pt.y;
	dx01 = xIn - x1;
	if(dx01 == 0) return pt.y;
	x2 = (KyInt64)other_pt.x;
	y2 = (KyInt64)other_pt.y;

	dx21 = x2 - x1;
	dy21 = y2 - y1;

	num = dx01*dy21;
	denum = dx21;
	if (denum == 0) 
	{
		return KyInt32MAXVAL - 1;
	}

	roundedResult = RoundedDivide(num, denum); 
	return (KyInt32)(roundedResult + y1);
}

// TODO: Return INT instead of FLOAT...
// cast from int to float to int = BAD - will LHS on PPC... 
static inline KyFloat32 EvalAtYforX_float(KyInt32 yIn, const Kaim::Vec2i& pt, const Kaim::Vec2i& other_pt) 
{ 
	KyInt32	/*evalAtXforYret,*/ x1, y1, dy01, dy21, 
		dx21, x2, y2;
	if(pt.x == other_pt.x)
		return (KyFloat32)pt.x;
	x1 = pt.x;
	y1 = pt.y;
	dy01 = yIn - y1;
	if(dy01 == 0) return (KyFloat32)x1;
	x2 = other_pt.x;
	y2 = other_pt.y;

	dx21 = x2 - x1;
	dy21 = y2 - y1;


	return (((KyFloat32)dy01 * (KyFloat32)dx21) / (KyFloat32)dy21 + (KyFloat32)x1);
}

// Returns the result of the division num/denum, rounded to the closest integer. 
// example: 1/3 returns 0
//        :-1/3 returns 0
//        : 1/2 returns 0
//        :-1/2 returns -1
//        : 2/3 returns 1
//        :-2/3 returns -2 and so on. 
KY_INLINE KyInt32 RoundedDivide(KyInt64 num, KyInt64 denum)
{
	KyInt64 doubleNum = num * 2; 
	KyInt64 truncatedResult = doubleNum/denum; 
	// On x86 the remainder is a by-product of the division itself so any half-decent compiler should be able to just use it (and not perform a div again). 
	// This is probably done on other architectures too.
	KyInt64 remainder = doubleNum%denum;
	// Determine if result is >0 BEFORE dividing. Otherwise If truncated result is 0, there is no way to know the real sign
	// of the result. 
	KyInt64 sign = (num ^ denum); //sign bit will have the correct value with a XOR
	KyInt64 truncationValue = Lsel(sign, 1, -1); // 1 if >= 0; -1 otherwise
	truncationValue = remainder == 0 && truncationValue > 0 ? truncationValue - 1 : truncationValue;
	// here truncation value is: 
	// * 1 if the sign is > 0 and remainder != 0
	// * 0 if the sign is < 0 or the sign > 0 && remainder = 0
	// * -1 if the sign is < 0 and the remainder is 0
	return (KyInt32)( (truncatedResult + truncationValue) / 2);
}

//Returns the X value of a point at a given Y on line [pt;other_pt].
//Result is rounded to the closest int inferior to the actual position. 
//eg: 
//y_float = 1.2f => 1
//y_float = 0.8f => 0
//y_float = -0.1f => -1
//y_float = -0.9f => -1
//y_float = -1.0001f => -2
//Computation is carried out in 64 bit and should not overflow. 
//If input values are not too big (< KyInt16MaxVal), _Int32 method should be preferred. 
static inline KyInt32 EvalAtYforX_Int64(KyInt32 yIn, const Kaim::Vec2i pt, const Kaim::Vec2i other_pt) 
{ 
	KyInt64	x1, y1, dy01, dy21, 
		dx21, x2, y2, num, denum, truncatedResult, remainder;
	if(pt.x == other_pt.x)
		return (KyInt32)pt.x;
	x1 = (KyInt64)pt.x;
	y1 = (KyInt64)pt.y;
	dy01 = yIn - y1;
	if(dy01 == 0) return pt.x;
	x2 = (KyInt64)other_pt.x;
	y2 = (KyInt64)other_pt.y;

	dx21 = x2 - x1;
	dy21 = y2 - y1;

	num = dy01*dx21;
	denum = dy21;
	if (denum == 0) 
	{
		return KyInt32MAXVAL - 1;
	}

	truncatedResult = num/denum; 
	// On x86 the remainder is a by-product of the division itself so any half-decent compiler should be able to just use it (and not perform a div again). 
	// This is probably done on other architectures too.
	remainder = num%denum;
	// Determine if result is >0 BEFORE dividing. Otherwise If truncated result is 0, there is no way to know the real sign
	// of the result. 
	KyInt64 sign = num ^ denum; //sign bit will have the correct value with a XOR
	KyInt64 truncationValue = (sign >> 63) & (remainder != 0); // 1 iff sign < 0 && remainder is different from 0. = 0 otherwise
	assert(truncatedResult + x1 <= KyInt32MAXVAL && truncatedResult + x1 > -KyInt32MAXVAL);
	return (KyInt32)(truncatedResult + x1 - truncationValue);
}


inline KyInt32 GetSign(KyInt32 i) { return Kaim::Isel(i, 1, -1); }

// Returns the closest Int whose value is equal or below the exact result of numerator/denominator
// 1/10 will return 0
// -1/10, -9/10 and obviously -10/10 will return -1 
// and so on... 
inline KyInt64 DivideTruncatedBelow(KyInt64 numerator, KyInt64 denominator)
{

	KyInt64 rawResult = numerator/denominator;
	// On x86 the remainder is a by-product of the division itself so any half-decent compiler should be able to just use it (and not perform a div again). 
	// This is probably done on other architectures too.
	KyInt64 remainder = numerator%denominator;
	// keep the absolute value of remainder
	remainder = Lsel(remainder, remainder, -remainder);
	// Determine if result is >0 BEFORE dividing. Otherwise If truncated result is 0, there is no way to know the real sign
	// of the result. 
	KyInt64 sign = Kaim::Lsel(numerator, 1, -1) * Kaim::Lsel(denominator, 1, -1);
	assert(rawResult <= KyInt32MAXVAL && rawResult > -KyInt32MAXVAL);
	return (KyInt32)Lsel(sign*remainder, rawResult, rawResult - 1);
}

} // namespace IntegerSegment
} //namespace Kaim


