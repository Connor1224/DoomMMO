/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_BoundaryPolygon_H
#define GwNavGen_BoundaryPolygon_H


#include "gwnavgeneration/boundary/boundarytypes.h"
#include "gwnavgeneration/boundary/boundaryedge.h"
#include "gwnavgeneration/common/stlvector.h"


namespace Kaim
{


class BoundaryOrderedSimplifyPolyline
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BoundaryOrderedSimplifyPolyline()
		: m_polyline(KY_NULL), m_order(Boundary::Order_Unset) {}

	BoundaryOrderedSimplifyPolyline(BoundarySimplifyPolyline* polyline, Boundary::Order order)
		: m_polyline(polyline), m_order(order) {}

public:
	BoundarySimplifyPolyline* m_polyline;
	Boundary::Order m_order;
};


class BoundaryContour
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	void Init(KyUInt32 index);

	void ComputeContourWinding();

public:
	KyUInt32 m_index;
	KyUInt32 m_edgeCount;
	BoundaryEdge* m_begin;
	Boundary::ContourWinding m_winding;
	Boundary::Color m_leftColor;
	TlsStlVector<BoundaryOrderedSimplifyPolyline> m_orderedSimplifyPolylines;
};


class BoundaryPolygon
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BoundaryPolygon()
	{
		Init(KyUInt32MAXVAL, Boundary::Color_Unset);
	}

	void Init(KyUInt32 index, Boundary::Color leftColor)
	{
		m_index = index;
		m_leftColor = leftColor;
		m_exterior = KY_NULL;
	}

public:
	KyUInt32 m_index;
	Boundary::Color m_leftColor;
	BoundaryContour* m_exterior;
	TlsStlVector<BoundaryContour*> m_holes;
};


}


#endif
