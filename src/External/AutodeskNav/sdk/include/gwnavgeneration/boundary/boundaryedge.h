/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_BoundaryEdge_H
#define GwNavGen_BoundaryEdge_H


#include "gwnavgeneration/boundary/boundarytypes.h"
#include "gwnavgeneration/boundary/boundarypixel.h"


namespace Kaim
{

class BoundaryVertex;
class BoundaryPixel;
class BoundaryContour;
class BoundarySimplifyPolyline;
class BoundarySimplifiedEdge;


class BoundaryEdge
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	void Init(Boundary::CardinalDir dir, Boundary::EdgeType type, bool isInside, KyUInt32 leftColor, KyUInt32 index);

	void ComputeNextEdge();

	bool IsInContour() { return m_contour != KY_NULL; }

	bool IsOutside() { return m_isOutside; }

	bool HasDiscontinuityVertex () const;

public:
	KyUInt32 m_index;

	Boundary::CardinalDir m_dir;
	Boundary::EdgeType m_type;
	BoundaryVertex* m_vertex[2];
	KyUInt32 m_leftColor;
	bool m_isOutside;
	KyUInt32 m_leftPixelFeatureTag;
	BoundaryEdge* m_pair;

	// contour related
	BoundaryContour* m_contour;
	BoundaryEdge* m_next;

	// simplifyPolyline related
	BoundarySimplifyPolyline* m_simplifyPolyline;
	Boundary::Order m_simplifyPolylineOrder;

	// simplifiedEdge related
	BoundarySimplifiedEdge* m_simplifiedEdge;
};

}


#endif
