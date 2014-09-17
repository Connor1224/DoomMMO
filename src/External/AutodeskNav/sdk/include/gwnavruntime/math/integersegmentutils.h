/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef Navigation_IntegerSegmentUtils_H
#define Navigation_IntegerSegmentUtils_H

#include "gwnavruntime/navmesh/intersections.h"


namespace Kaim
{

// Order cuts depending on their distance from edge start
class CutSorter
{
public:
	CutSorter(const Vec2i& edgeStart) : m_edgeStart(edgeStart) {}
	bool operator() (const Vec2i& cut1, const Vec2i& cut2) const
	{
		return (cut1 - m_edgeStart).GetSquareLength() > (cut2 - m_edgeStart).GetSquareLength();
	}

public:
	Vec2i m_edgeStart;
};

namespace IntegerSegment
{

	static KY_INLINE KyInt32 RoundedDivide(KyInt64 num, KyInt64 denum);
	// Generic template functions
	// returns true iff pt coordinate is between start and end
	template <typename T> //T E { Vec{2,3}i, Vec2LL } 
	static KY_INLINE bool Between(const T& pt, const T& edgeStart, const T& edgeEnd);

	// returns true if [ edgeStart, edgeEnd ] is vertical
	template <typename T> //T E { Vec{2,3}i, Vec2LL } 
	static KY_INLINE bool IsVertical(const T& edgeStart, const T& edgeEnd);
	
	// returns true if [ edgeStart, edgeEnd ] is horizontal
	template <typename T> //T E { Vec{2,3}i, Vec2LL } 
	static KY_INLINE bool IsHorizontal(const T& edgeStart, const T& edgeEnd);

	// returns true if edge1 with derivative (dx1, dy1) has a slope inferior to edge2 (dx2, dy2)
	template <typename T> //T E { Int32/64 } 
	static bool LessSlopeUnit(T dx1, T dy1, T dx2, T dy2);
	//static inline bool LessSlope_hp(T dx1, T dy1, T dx2, T dy2);

	template <typename T>  //T E { Vec{2,3}i, Vec2LL }
	static KY_INLINE bool LessSlope(KyInt32 x, KyInt32 y, const Vec2i& pt1, const Vec2i& pt2);
	
	template <typename T>  //T E { Vec{2,3}i, Vec2LL }
	static KY_INLINE bool EqualSlopes(KyInt32 x, KyInt32 y, const Vec2i& pt1, const Vec2i& pt2);

	
	//return -1 below, 0 on and 1 above line
	template <typename T>  
	static KY_INLINE int OnAboveOrBelow(const Vec2i& pt, const Vec2i& edgeStart, const Vec2i& edgeEnd);

	template <typename T> 
	bool SegmentVsSegment2d(const T& a, const T& b, const T& c, const T& d);

	// NB1: When relevant, the intersection point is snapped to the lower left corner of the pixel where the 
	// "Real" (precise) intersection occurs. This is not a simple integer conversion by truncation.
	bool SegmentVsSegment2d_hp(const Vec2i& a, const Vec2i& b, const Vec2i& c, const Vec2i& d, Vec2i& intersection);

	bool SegmentVsSegment2d_rounded(const Vec2i& a, const Vec2i& b, const Vec2i& c, const Vec2i& d, Vec2i& intersection);

} // namespace IntegerSegment

} //namespace Kaim


#include "gwnavruntime/math/integersegmentutils.inl"


#endif // Navigation_IntegerSegmentUtils_H
