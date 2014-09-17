/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef Navigation_NavFloorSweeplineTypes_H
#define Navigation_NavFloorSweeplineTypes_H


#include <assert.h>
#include "gwnavruntime/math/integersegmentutils.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"
#include "gwnavruntime/containers/bitfield.h"
#include "gwnavruntime/database/navtag.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmembitfield.h"
#include "gwnavruntime/querysystem/workingmemory.h"


namespace Kaim
{

typedef Kaim::KyArray<MergerRawInputEdgePiece> NavFloorSweeplineInputElements;
class TagVolumeSweepInput
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public:
	void CreateFromEdgeIntersections(const RawPolygonEdgeIntersections& intersections)
	{
		KyUInt32 indexCounter = 0;
		for (KyUInt32 edgeIdx = 0; edgeIdx < intersections.m_edges.GetCount(); ++edgeIdx)
		{
			const MergerRawInputEdge* edge = intersections.m_edges[edgeIdx];
			EdgeCutList::ConstIterator iter = intersections.m_cuts[edgeIdx].Begin();
			for (; iter != intersections.m_cuts[edgeIdx].End(); ++iter)
			{
				EdgeCutList::ConstIterator next = iter;
				next++;
				if (next == intersections.m_cuts[edgeIdx].End())
				{
					break;
				}
				MergerRawInputEdgePiece newElem;
				Vec2i start = *iter;
				Vec2i end = *next;
				KyInt32 key = start < end ? 1 : -1;
				if (key < 0)
				{
					newElem.m_edgePiece = MergerRawInputEdgeSubSegment(end, start);
				}
				else
				{
					newElem.m_edgePiece = MergerRawInputEdgeSubSegment(start, end);
				}
				if (start.x == end.x)
				{
					key *= -1;
				}
				newElem.m_parentEdge = edge;
				newElem.m_index = indexCounter++;
				newElem.m_count = key;
				m_inputElements.PushBack(newElem);
			}
		}
	}

public:
	NavFloorSweeplineInputElements m_inputElements;

};


class TagVolumeSweepResult
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)

public:
	inline void Insert(MergerRawInputEdgePiece& elem, KyUInt32 navTagIdx)
	{
		MergerRawInputEdgePiece newElem = elem;
		// Make sure we don't have edges outside of the cell box (0-255 max)
		assert(elem.m_edgePiece.m_end.x >= 0 && elem.m_edgePiece.m_end.x <= KyUInt8MAXVAL && elem.m_edgePiece.m_end.y >= 0 && elem.m_edgePiece.m_end.y <= KyUInt8MAXVAL);
		assert(elem.m_edgePiece.m_start.x >= 0 && elem.m_edgePiece.m_start.x <= KyUInt8MAXVAL && elem.m_edgePiece.m_start.y >= 0 && elem.m_edgePiece.m_start.y <= KyUInt8MAXVAL);
		if (elem.m_edgePiece.m_end < elem.m_edgePiece.m_start)
		{
			//swap
			newElem.m_edgePiece.m_end = elem.m_edgePiece.m_start;
			newElem.m_edgePiece.m_start = elem.m_edgePiece.m_end;
			newElem.m_count *= -1;
		}
		m_rawResults.PushBack(newElem);
		KyUInt32 thisNavTagIndexPos = 0;
		for (; thisNavTagIndexPos < m_navTagIndexes.GetCount(); ++thisNavTagIndexPos)
		{
			if(m_navTagIndexes[thisNavTagIndexPos] == navTagIdx)
			{
				m_elemByNavTag[thisNavTagIndexPos].PushBack(m_rawResults.GetCount() - 1);
				break;
			}
		}
		if (thisNavTagIndexPos == m_navTagIndexes.GetCount())
		{
			m_navTagIndexes.PushBack(navTagIdx);
			m_elemByNavTag.PushDefault().PushBack(m_rawResults.GetCount() - 1);
			assert(m_elemByNavTag.GetCount() == m_navTagIndexes.GetCount());
		}
	}

public:
	KyArray<MergerRawInputEdgePiece> m_rawResults;
	KyArray<KyUInt32> m_navTagIndexes;
	KyArray<KyArray<KyUInt32> > m_elemByNavTag;
};


//NavFloorSweepline comparator functor

struct SweeplineHalfEdgeProperty
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)

public:
	SweeplineHalfEdgeProperty():
		m_halfEdge(KY_NULL),
		m_count(0)
	{}
	SweeplineHalfEdgeProperty(const MergerRawInputEdgePiece& he):
		m_halfEdge(&he),
		m_count(he.GetCount())
		{}
	KyUInt32 GetCompositeIndex() const 
	{ 
		return m_halfEdge->ParentProperty().GetCompositeIndex();
	}
	bool IsFromSameInputContour(const SweeplineHalfEdgeProperty& other) { return GetCompositeIndex() == other.GetCompositeIndex(); }
	bool operator <(const SweeplineHalfEdgeProperty& rhs) const { return GetCompositeIndex() < rhs.GetCompositeIndex(); }
	bool operator ==(const SweeplineHalfEdgeProperty& rhs) const { return GetCompositeIndex() == rhs.GetCompositeIndex(); }

public:
	const MergerRawInputEdgePiece* m_halfEdge;
	KyInt32 m_count;
};
typedef SharedPoolList<SweeplineHalfEdgeProperty> SweeplineHalfEdgePropertyList;


class TagVolumeHalfEdgeWithPropertyList
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	TagVolumeHalfEdgeWithPropertyList(){}
	TagVolumeHalfEdgeWithPropertyList(MergerRawInputEdgePiece& edge, SweeplineHalfEdgePropertyList* properties)
		: m_edge(&edge), m_properties(properties) {}

public:
	MergerRawInputEdgePiece* m_edge;
	SweeplineHalfEdgePropertyList* m_properties;
};

} //namespace Kaim

#endif // Navigation_NavFloorSweeplineTypes_H
