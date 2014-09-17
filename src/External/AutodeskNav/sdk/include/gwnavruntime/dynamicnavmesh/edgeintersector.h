/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef KAIM_EDGE_INTERSECTOR_H
#define KAIM_EDGE_INTERSECTOR_H

// primary contact: LASI - secondary contact: NONE

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/math/vec3i.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"

namespace Kaim
{
	// For Debug render
	class ScopedDisplayList;
	class VisualColor;
}

namespace Kaim
{
//----------------------------------------------------------------------
// Compute all the intersections between all the edges passed as input.
// The intersection points will be snapped to an integer grid and injected as "Cuts" on the original edges supplied.
// The snapping is done in a way that prevents spurious intersections that normally result from such process.
// This approach is similar to:
//----------------------------------------------------------------
// Hobby, J. - 1993
// Practical segment intersection with finite precision output.
// Technical Report 93/2-27, Bell Laboratories (Lucent Technologies)
//------------------------------------------------------------------
class EdgeIntersector
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	EdgeIntersector() : m_debugDisplayListBeforeSnap(KY_NULL), m_debugDisplayListAfterSnap(KY_NULL){}
public:
	// DEBUG display settings
	// displayListBeforeSnap and displayListAfterSnap pointers are use to distinguish what we want to display
	void SetDebugDisplayLists(KyFloat32 rasterPrecision,const Kaim::Vec2i& cellOffset, ScopedDisplayList* displayListBeforeSnap, ScopedDisplayList* displayListAfterSnap)
	{
		m_rasterPrecision = rasterPrecision;
		m_cellOffset = cellOffset;
		m_debugDisplayListBeforeSnap = displayListBeforeSnap;
		m_debugDisplayListAfterSnap = displayListAfterSnap;
	}

	KyResult ComputeIntersections(const KyArray<MergerRawInputEdge>& edges, KyArray<MergerRawInputEdgePiece>& intersectedEdges);

public:
	// DEBUG Render using ScopedDisplayList
	KyResult CheckResult(const RawPolygonEdgeIntersections& intersections, ScopedDisplayList* displayList);

private:
	KyResult IntersectionsToDisplayList(const RawPolygonEdgeIntersections& intersections, const KyArray<Vec2i>& hotPoints, ScopedDisplayList& displayList, VisualColor color = VisualColor::White);
	KyResult EdgeArrayToDisplayList(const KyArray<MergerRawInputEdge>& edges, ScopedDisplayList& displayList, const VisualColor& color);

private:
	ScopedDisplayList* m_debugDisplayListBeforeSnap;
	ScopedDisplayList* m_debugDisplayListAfterSnap;
	KyFloat32 m_rasterPrecision;
	Kaim::Vec2i m_cellOffset;
};

} // namespace Kaim



#endif // KAIM_EDGE_INTERSECTOR_H
