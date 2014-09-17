/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/




// primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_BoundaryEarSimplifier_H
#define GwNavGen_BoundaryEarSimplifier_H

#include "gwnavruntime/base/types.h"
#include "gwnavgeneration/common/stllist.h"
#include "gwnavgeneration/boundary/boundarygraph.h"

namespace Kaim
{


class BoundaryEarSimplifier
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
private: 

	enum Polygon_Feature_Type
	{
		POLYGON_EAR,
		POLYGON_MOUTH
	};

	enum Simplification_Direction
	{
		SIMPLIFY_TOWARD_INTERIOR,
		SIMPLIFY_TOWARD_EXTERIOR
	};

public:
	BoundaryEarSimplifier(BoundaryGraph* boundaryGraph, KyFloat32 pixelInwardTolerance, KyFloat32 pixelOutwardTolerance) :
		m_boundaryGraph(boundaryGraph),
		m_pixelInwardTolerance(pixelInwardTolerance),
		m_pixelOutwardTolerance(pixelOutwardTolerance)
	{}

	KyResult SimplifyBoundaries();

private:
	// 1. For each polygon ...
	KyResult SimplifyPolygon(BoundarySimplifiedPolygon& polygon);
	// 2. For each contours (exterior + holes[])...
	KyResult SimplifyContour(BoundarySimplifiedContour& contour,  KyArrayPOD<BoundaryVertex*>& allVertices, Simplification_Direction direction);

	bool IsSpecialPolygonFeature(const Vec2i& A, const Vec2i& B, const Vec2i& C, const KyArrayPOD<BoundaryVertex*>& vertices, Polygon_Feature_Type feature_type);

private:
	BoundaryGraph* m_boundaryGraph;
	KyFloat32 m_pixelInwardTolerance; // in pixels (eg 0.5f means 1/2 pixelSizeInMeters)
	KyFloat32 m_pixelOutwardTolerance; // in pixels (eg 0.5f means 1/2 pixelSizeInMeters)
};

}


#endif //GwNavGen_BoundaryEarSimplifier_H

