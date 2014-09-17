/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef KAIM_BOX_CLIPPER_H
#define KAIM_BOX_CLIPPER_H

// primary contact: LASI - secondary contact: NONE

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/box2i.h"
#include "gwnavruntime/math/vec2i.h"

namespace Kaim
{

// Based on Sutherland Hodgman clipping algorithm:
// cf. http://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm
// Might create coincident edges if the subject is concave outside of clipping box
// Temporary limitation: Input values should be within range [-KyInt16MaxVal, KyInt16MaxVal] 
class BoxPolylineClipper
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
private:
	enum BoxEdgeType
	{
		LEFT_BOX_EDGE, TOP_BOX_EDGE, RIGHT_BOX_EDGE, BOTTOM_BOX_EDGE
	};
public:
	BoxPolylineClipper(const Box2i& clippingBox) : m_clippingBox(clippingBox) {}

	KyResult ClipPolyline(const KyArray<Vec2i>& originalPolyline, KyArray<Vec2i>& clippedPolyline) const;

private:
	void BreakPolylineOnBox(const KyArray<Vec2i>& originalPolyline, KyArray<Vec2i>& brokenPolyline) const;
	bool IsOutside(const Vec2i& pt, BoxEdgeType edgeType) const;
	Vec2i GetIntersect(BoxEdgeType type, const Vec2i& segStart, const Vec2i& segEnd) const;

public:
	Box2i m_clippingBox;
};

}

#endif //KAIM_BOX_CLIPPER_H
