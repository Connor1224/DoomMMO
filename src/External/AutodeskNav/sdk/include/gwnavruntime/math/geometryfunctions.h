/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_GeometryFunctions_H
#define Navigation_GeometryFunctions_H

#include "gwnavruntime/base/types.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/vec2i.h"

namespace Kaim
{
class Vec2f;

enum PolygonWinding
{
	POLYGON_IS_CCW,
	POLYGON_IS_CW,
	POLYGON_UNKNOWN_WINDING,
};

namespace GeometryFunctions
{
// Tests whether or not a specified point is inside the area defined by a polyline in two 
// dimensions, ignoring their altitude.
// \param p					The point to test.
// \param points			The list of points in the polyline. This list must be in clockwise order,
// 							as seen from above. The list must also be closed: its last point must be the
// 							same as its first point.
// \param count				The number of points in the points array.
// \return true if the point is inside the polyline or on the borders of the polyline, or false otherwise.
bool IsInside2d_Polyline(const Vec2f& p, const Vec2f* points, KyUInt32 count);

bool IsInside2d_Polyline(const Vec2i& p, const Vec2i* points, KyUInt32 count);

PolygonWinding ComputePolygonWinding(const KyArray<Vec2i>& polygon); 
PolygonWinding ComputePolygonWinding(const Vec2f* polygon, UPInt count);

bool IsTwisted(const Vec2f* polygon, UPInt count);

} //namespace GeometryFunctions

} //namespace Kaim

#endif // Navigation_GeometryFunctions_H
