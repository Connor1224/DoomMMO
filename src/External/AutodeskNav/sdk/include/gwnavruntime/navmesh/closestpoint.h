/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_ClosestPoint_H
#define Navigation_ClosestPoint_H

#include "gwnavruntime/navmesh/navmeshtypes.h"

namespace Kaim
{

class ClosestPoint // class with static functions only
{
public :
// oneShape1VsShape2 = compute the closest point of Shape1 from Shape2

// Compute the point of the Segment [ab] the is closest from the Point p
static void OnSegmentVsPoint2d(const Vec3f& a, const Vec3f& b, const Vec3f& p, Vec3f& closestPointOnSegment, KyFloat32& squareDist);
static void OnSegmentVsPoint2d(const CoordPos64& a, const CoordPos64& b, const CoordPos64& p, CoordPos64& closestPointOnSegment, KyInt64& squareDist);

// Compute the point the Triangle [v1, v2, v3] the is closest from the Point p
static void OnTriangleVsPoint2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& p, Vec3f& closestPointOnTriangle, KyFloat32& squareDist);

// Compute the point the Segment [v1v2] the is closest from the Segment[ab]
static void OnSegmentVsSegment2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& a, const Vec3f& b, const KyFloat32 distCast, Vec3f& closestPointonTriangle, KyFloat32& squareDist);
// Compute the point the Triangle [v1, v2, v3] the is closest from the Segment [ab]
static void OnTriangleVsSegment2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& a, const Vec3f& b, const KyFloat32 distCast, Vec3f& closestPointonTriangle, KyFloat32& squareDist);

// Compute the point the Segment [v1v2] the is closest from the Disk (center, radius)
static void OnSegmentVsDisk2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& center, const KyFloat32 radius, const Vec2f& normalizedDir2D, const KyFloat32 distCast, Vec3f& closestPointonTriangle, KyFloat32& squareDist);
// Compute the point the Triangle [v1, v2, v3] the is closest from the Disk (center, radius)
static void OnTriangleVsDisk2d(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3, const Vec3f& center, const KyFloat32 radius, const Vec2f& normalizedDir2D, const KyFloat32 distCast, Vec3f& closestPointonTriangle, KyFloat32& squareDist);

};

}

#endif // Navigation_ClosestPoint_H

