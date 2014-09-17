/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_Intersections_H
#define Navigation_Intersections_H

#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/math/orientedbox2d.h"
#include "gwnavruntime/math/box2f.h"

namespace Kaim
{


class Intersections // class with static functions only
{
public :

// compute if vertical line that contains p intersects the plane defined by the triangle
static bool IsPointInsideTriangle2d(const Vec3f& p,const Vec3f& v1, const Vec3f& v2, const Vec3f& v3);
static bool IsPointInsideTriangle2d(const CoordPos64& p,const CoordPos64& v1, const CoordPos64& v2, const CoordPos64& v3);
static bool IsPointInsideTriangle2d(const CoordPos& p,const CoordPos& v1, const CoordPos& v2, const CoordPos& v3);
static bool IsOnTheLeftOfTheEdge2d(const CoordPos& p, const CoordPos& v1, const CoordPos& v1v2);
static bool IsStrictlyOnTheLeftOfTheEdge2d(const CoordPos& p, const CoordPos& v1, const CoordPos& v1v2);
static bool IsOnTheLeftOfTheEdge2d(const CoordPos64& p, const CoordPos64& v1, const CoordPos64& v1v2);
static bool IsStrictlyOnTheLeftOfTheEdge2d(const CoordPos64& p, const CoordPos64& v1, const CoordPos64& v1v2);

// Intersections
// we test the intersection between convex polylines by using the Separated-Axes Theorem
static bool AABBVsAABB2d(const Box2LL& box1, const Box2LL& box2);
static bool AABBVsAABB2d(const Box2i& box1, const Box2i& box2);
static bool AABBVsAABB2d(const Box2f& box1, const Box2f& box2);
static bool AABBVsAABB3d(const Box3f& box1, const Box3f& box2);
static bool AABBVsAABB2d(const Box2f& box1, const Box3f& box2);

static bool AABBVsOrientedBox2d(const OrientedBox2d& orientedBox2d, const Box2f& box);
static bool AABBVsOrientedBox2d(const CoordPos64& o1, const Vec2LL& lengthVec, const Vec2LL& widthVec, const CoordBox64& aABbox);

//static bool AABBVsOrientedBox2d(const OrientedBox2d& orientedBox2d, const Box3f& box)
//{
//	return AABBVsOrientedBox2d(orientedBox2d, Box2f(box.m_min.x, box.m_min.y, box.m_max.x, box.m_max.y));
//}

static bool SegmentVsDisk2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, KyFloat32 radius);
static bool SegmentVsDisk2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, KyFloat32 radius, Vec3f& intersection);

static bool SegmentVsOrientedBox2d(const Vec3f& v1, const Vec3f& v2, const OrientedBox2d& orientedBox2d);
static bool SegmentVsOrientedBox2d(const CoordPos64& v1, const CoordPos64& v2, const CoordPos64& o1, const Vec2LL& lengthVec, const Vec2LL& widthVec);

// used during the Disk propagation. Tells is the segment is inside the box and the extended box
static bool SegmentVsExtendedOrientedBox2d(const Vec3f& v1, const Vec3f& v2, const OrientedBox2d& orientedBox2d, const KyFloat32 extralength, bool& outsideExtendedBox);

static bool SegmentVsAABB2d(const Vec3f& v1, const Vec3f& v2, const Box2f& aabb2d);
static bool SegmentVsAABB2d(const CoordPos64& v1, const CoordPos64& v2, const CoordBox64& aabb2d);

static bool SegmentVsCapsule2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& center, const KyFloat32 radius, const Vec2f& normalizedDir2D, const KyFloat32 distCast);
// this function should be call by a visitor to avoid creating and computing orientedBox2d, center1 and center2 at each call as in the first function SegmentVsCapsule2d
static bool SegmentVsCapsule2d(const Vec3f& v1, const Vec3f& v2, const OrientedBox2d& orientedBox2d, const Vec3f& center1, const Vec3f& center2, const KyFloat32 radius);

// test intersection between a segment and a Capsule that is truncated on the left and of the right along the middle axis.
static bool SegmentVsCrossSectionCapsule2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& center, const KyFloat32 radius, const Vec2f& normalizedDir2D, const KyFloat32 distCast, const KyFloat32 distOnRight, const KyFloat32 distOnLeft);
// this function should be call by a visitor to avoid creating and computing 2 BoxBostacle and the center of the end Disk of the capsule
static bool SegmentVsCrossSectionCapsule2d(const Vec3f& v1, const Vec3f& v2, const OrientedBox2d& box, const Vec3f& centerOfEndDisk, const KyFloat32 radius, const OrientedBox2d& boxOfEndDisk);

static bool SegmentVsTriangle2d(const Vec2f& a, const Vec2f& b, const Vec2f& v1, const Vec2f& v2, const Vec2f& v3);
static bool SegmentVsTriangle2d(const CoordPos64& v1, const CoordPos64& v2, const CoordPos64& t1, const CoordPos64& t2, const CoordPos64& t3);

static bool TriangleVsOrientedBox2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const OrientedBox2d& orientedBox2d);
static bool TriangleVsOrientedBox2d(const CoordPos64& v1, const CoordPos64& v2, const CoordPos64& v3, const CoordPos64& o1, const Vec2LL& lengthVec, const Vec2LL& widthVec);

//Not used. Be careful, this function has not been unitTested yet
static bool TriangleVsTriangle2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& u1, const Vec3f& u2, const Vec3f& u3);


static bool TriangleVsAABB2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Box2f& box);
static bool OverlappingOrientedBox2dVsTriangle3D(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const OrientedBox2d& orientedBox2d, const KyFloat32 toleranceBelow, const KyFloat32 toleranceAbove);

static bool SegmentVsSegment2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d);
static bool SegmentVsSegment2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d, Vec3f& intersection);

static bool LineVsLine2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d);
static bool LineVsLine2d(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d, Vec3f& intersection);

static bool LineVsLine2d(const CoordPos64& a, const CoordPos64& b, const CoordPos64& c, const CoordPos64& d);
static bool LineVsLine2d(const CoordPos64& a, const CoordPos64& b, const CoordPos64& c, const CoordPos64& d, CoordPos64& intersection);

// these two functions are test of intersection with a vertical line

// compute the altitude of point of intersection the vertical line that contains p and the plane defined by the triangle
static KyFloat32 ComputeAltitudeOfPointInPlane(const Vec2f& p,const Vec3f& planePoint, const Vec3f& planeNormal);
static KyFloat32 ComputeAltitudeOfPointInTriangle(const Vec3f& p,const Vec3f& v1, const Vec3f& v2, const Vec3f& v3);
static KyFloat32 ComputeAltitudeOfPointInTriangle(const Vec2f& p,const Vec3f& v1, const Vec3f& v2, const Vec3f& v3);

// triangle v1, v2, v3 is CounterClockWise
static KyFloat32 ComputeInCircumCircleDeterminant(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& p);
static bool IsPointInsideCircumCircle2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& p);
static bool IsPointStrictlyInsideCircumCircle2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& p);

static KyInt64 ComputeInCircumCircleDeterminant(const CoordPos& a, const CoordPos& b, const CoordPos& c, const CoordPos& d);
static bool IsPointInsideCircumCircle2d(const CoordPos& v1, const CoordPos& v2, const CoordPos& v3, const CoordPos& p);
static bool IsPointStrictlyInsideCircumCircle2d(const CoordPos& v1, const CoordPos& v2, const CoordPos& v3, const CoordPos& p);

};

KY_INLINE bool Intersections::IsPointInsideCircumCircle2d(const CoordPos& v1, const CoordPos& v2, const CoordPos& v3, const CoordPos& p)
{
	return ComputeInCircumCircleDeterminant(v1, v2, v3, p) >= 0;
}

KY_INLINE bool Intersections::IsPointStrictlyInsideCircumCircle2d(const CoordPos& v1, const CoordPos& v2, const CoordPos& v3, const CoordPos& p)
{
	return ComputeInCircumCircleDeterminant(v1, v2, v3, p) > 0;
}


inline bool Intersections::IsOnTheLeftOfTheEdge2d(const CoordPos& p, const CoordPos& v1, const CoordPos& v1v2)
{
	const CoordPos v1p(p.x - v1.x, p.y - v1.y); // 31 Bits

	KY_LOG_WARNING_IF( (v1v2.x >= 1 << 15 || -v1v2.x >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( (v1v2.y >= 1 << 15 || -v1v2.y >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.x >= 1 << 15 || - v1p.x >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.y >= 1 << 15 || - v1p.y >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));

	const KyInt32 crossProductv1v2 =  CrossProduct(v1v2, v1p); // 2 * 15 + 1 = 31 bits
	const KyInt32 resultv1v2_1 = Isel(crossProductv1v2, 1, 0);
	const KyInt32 resultv1v2_2 = Isel(-crossProductv1v2, 0, 1);

	const KyInt32 isOnLeft = Isel(-v1v2.x, Isel(v1v2.x, Isel(-v1v2.y, resultv1v2_2, resultv1v2_1), resultv1v2_2), resultv1v2_1); // 0 ou 1
	return isOnLeft != 0;

}

inline bool Intersections::IsStrictlyOnTheLeftOfTheEdge2d(const CoordPos& p, const CoordPos& v1, const CoordPos& v1v2)
{
	const CoordPos v1p(p.x - v1.x, p.y - v1.y); // 31 Bits

	KY_LOG_WARNING_IF( (v1v2.x >= 1 << 15 || -v1v2.x >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( (v1v2.y >= 1 << 15 || -v1v2.y >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.x >= 1 << 15 || - v1p.x >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.y >= 1 << 15 || - v1p.y >= 1 << 15), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));

	return CrossProduct(v1v2, v1p) > 0;
}

inline bool Intersections::IsOnTheLeftOfTheEdge2d(const CoordPos64& p, const CoordPos64& v1, const CoordPos64& v1v2)
{
	const CoordPos64 v1p(p.x - v1.x, p.y - v1.y); // 31 Bits

	KY_LOG_WARNING_IF( (v1v2.x >= 1LL << 31 || -v1v2.x >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( (v1v2.y >= 1LL << 31 || -v1v2.y >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.x >= 1LL << 31 || - v1p.x >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.y >= 1LL << 31 || - v1p.y >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));

	const KyInt64 crossProductv1v2 =  CrossProduct(v1v2, v1p); // 2 * 15 + 1 = 31 bits
	const KyInt64 resultv1v2_1 = Lsel(crossProductv1v2, 1, 0);
	const KyInt64 resultv1v2_2 = Lsel(-crossProductv1v2, 0, 1);

	const KyInt64 isOnLeft = Lsel(-v1v2.x, Lsel(v1v2.x, Lsel(-v1v2.y, resultv1v2_2, resultv1v2_1), resultv1v2_2), resultv1v2_1); // 0 ou 1
	return isOnLeft != 0;

}

inline bool Intersections::IsStrictlyOnTheLeftOfTheEdge2d(const CoordPos64& p, const CoordPos64& v1, const CoordPos64& v1v2)
{
	const CoordPos64 v1p(p.x - v1.x, p.y - v1.y); // 31 Bits

	KY_LOG_WARNING_IF( (v1v2.x >= 1LL << 31 || -v1v2.x >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( (v1v2.y >= 1LL << 31 || -v1v2.y >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.x >= 1LL << 31 || - v1p.x >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF( ( v1p.y >= 1LL << 31 || - v1p.y >= 1LL << 31), ("input value require too many bits of precision. Potential computation overflow, result may be incoherent"));

	return CrossProduct(v1v2, v1p) > 0;
}

// Intersections
// we test the intersection between convex polylines by using the Separated-Axes Theorem
KY_INLINE bool Intersections::AABBVsAABB2d(const Box2LL& box1, const Box2LL& box2)
{
	const KyInt64 operand1 = Lsel(box2.m_max.x - box1.m_min.x, (KyInt64)1, (KyInt64)0); // operand1 == 1 if box2.m_max.x >= box1.m_min.x
	const KyInt64 operand2 = Lsel(box1.m_max.x - box2.m_min.x, (KyInt64)1, (KyInt64)0); // operand2 == 1 if box1.m_max.x >= box2.m_min.x
	const KyInt64 operand3 = Lsel(box2.m_max.y - box1.m_min.y, (KyInt64)1, (KyInt64)0); // operand3 == 1 if box2.m_max.y >= box1.m_min.y
	const KyInt64 operand4 = Lsel(box1.m_max.y - box2.m_min.y, (KyInt64)1, (KyInt64)0); // operand4 == 1 if box1.m_max.y >= box2.m_min.y
	return (operand1 & operand2 & operand3 & operand4) != (KyInt64)0;
}

inline bool Intersections::AABBVsAABB2d(const Box2i& box1, const Box2i& box2)
{
	const KyInt32 operand1 = Isel(box2.m_max.x - box1.m_min.x, (KyInt32)1, (KyInt32)0); // operand1 == 1 if box2.m_max.x >= box1.m_min.x
	const KyInt32 operand2 = Isel(box1.m_max.x - box2.m_min.x, (KyInt32)1, (KyInt32)0); // operand2 == 1 if box1.m_max.x >= box2.m_min.x
	const KyInt32 operand3 = Isel(box2.m_max.y - box1.m_min.y, (KyInt32)1, (KyInt32)0); // operand3 == 1 if box2.m_max.y >= box1.m_min.y
	const KyInt32 operand4 = Isel(box1.m_max.y - box2.m_min.y, (KyInt32)1, (KyInt32)0); // operand4 == 1 if box1.m_max.y >= box2.m_min.y
	return (operand1 & operand2 & operand3 & operand4) != (KyInt32)0;
}
}

#endif // Navigation_Intersections_H

