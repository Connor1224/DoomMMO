/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef Navigation_NavFloorSweepline_H
#define Navigation_NavFloorSweepline_H

#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/dynamicnavmesh/navfloorsweeplinetypes.h"

#include "gwnavruntime/math/integersegmentutils.h"
#include "gwnavruntime/visualdebug/visualdebugserver.h"
#include "gwnavruntime/dynamicnavmesh/triangulator.h"

#if 1
#include <utility>
#include <assert.h>
#endif

namespace Kaim
{
class DynamicNavTag;
class ScopedDisplayList;

KyUInt32 ComputeNavTagIndexFromPropertyList(const SweeplineHalfEdgePropertyList& propertySet);
bool AreListsEquals(const SweeplineHalfEdgePropertyList& list1, const SweeplineHalfEdgePropertyList& list2);
void TransferContent(SweeplineHalfEdgePropertyList& list1, SweeplineHalfEdgePropertyList& list2, bool clearList2 = true);

// This object is used to sweep a plane containing edges belonging to polygons and determine
// what's left and right of each edge in terms of polygons.
// This algorithm is based on boost polygon approach (http://www.boost.org/doc/libs/1_47_0/libs/polygon/doc/index.htm)
// and vatti clipping algorithm (http://en.wikipedia.org/wiki/Vatti_clipping_algorithm)
// In order to work properly this algorithm requires that:
// 1) Input edges DO NOT intersect except at their endpoints

class NavFloorSweepline
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)

public:
	typedef TagVolumeHalfEdgeWithPropertyList ScanDataElement;
	typedef SharedPoolList<ScanDataElement> ScanData;
	typedef ScanData::Iterator ScanDataIterator;
	typedef ScanData::ConstIterator ScanDataConstIterator;
	typedef ScanData::NodePool ScanDataPool;
	typedef SharedPoolList<Kaim::Vec2i> NavFloorSweeplineEndPointQueue;
	typedef NavFloorSweeplineEndPointQueue::NodePool EnPointPool;

public:
	enum BooleanOperationType
	{
		BOOLEAN_OR,
		CLIP_WITH_BORDER_AND_MERGE
	};
	typedef Pool<SweeplineHalfEdgePropertyList> PropertyPool;
	typedef Pool<SweeplineHalfEdgePropertyList>::Key PropertyKey;
	typedef Kaim::TriangulatorInputPolygon SimplePolygon;
	typedef Kaim::TriangulatorInputPolygonWithHoles PolygonWithHoles;

public:
	NavFloorSweepline(const KyArray<DynamicNavTag>* navtags = KY_NULL):
		m_propertyPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 64), 
		m_propertyListPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256), 
		m_scanDataPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256), 
		m_endPointPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256), 
		m_insertionSetPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256), 
		m_insertion_elementsPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256),
		m_removalSetPool(Memory::GetGlobalHeap(), MemStat_DynNavMesh, PoolChunkSize::SlotCount, 256)
	{
		m_scanData.SetNodePool(&m_scanDataPool);
		m_propertyListKeys.Reserve(256);
		m_navTags = navtags;
		m_justBefore = 0;
		m_endpointQueue.SetNodePool(&m_endPointPool);
		m_insertionSet.SetNodePool(&m_insertionSetPool);
		m_insertion_elements.SetNodePool(&m_insertion_elementsPool);
		m_removalSet.SetNodePool(&m_removalSetPool);
		m_displayList = KY_NULL;
	}
	~NavFloorSweepline()
	{
		for(KyUInt32 i = 0; i < m_propertyListKeys.GetCount(); ++i)
		{
			m_propertyListPool.Delete(m_propertyListKeys[i]);
		}
	}
	KyResult Scan(KyArray<MergerRawInputEdgePiece>& input, TagVolumeSweepResult& result);
	void SetDebugDisplayList(KyFloat32 rasterPrecision, const Vec2i& cellOffset, ScopedDisplayList* displayList)
	{
		m_rasterPrecision = rasterPrecision;
		m_cellOriginOffset = Kaim::Vec3i(cellOffset.x, cellOffset.y, 0);
		m_displayList = displayList;
	}

private:
	void EraseEndEvent(NavFloorSweeplineEndPointQueue::Iterator epqi);
	void RemoveRetiredEdges();
	
	void InsertSortedInScandata(const ScanDataElement& elem);
	void InsertSortedInEndPoints(const Vec2i& elem);
	ScanDataIterator LowerBoundInScandata(const MergerRawInputEdgeSubSegment& refElem);

	NavFloorSweeplineInputElements::Iterator HandleInputEvents(NavFloorSweeplineInputElements::Iterator begin, NavFloorSweeplineInputElements::Iterator end, TagVolumeSweepResult& result);
	void InsertNewEdges();
	void WriteOutEdge(TagVolumeSweepResult& result, const MergerRawInputEdgePiece& he, const SweeplineHalfEdgePropertyList& pm_left, const SweeplineHalfEdgePropertyList& pm_right);
	void FilterOutlineEdge(TagVolumeSweepResult& result, const MergerRawInputEdgePiece& edge, const SweeplineHalfEdgePropertyList& left, const SweeplineHalfEdgePropertyList& right);
	bool IsEdgeFullyInsideHole(const SweeplineHalfEdgePropertyList& leftSet, const SweeplineHalfEdgePropertyList& rightSet);
	bool IsEdgeInsideFloor(const SweeplineHalfEdgePropertyList& propertySet);
	bool IsHalfEdgeInHole(const SweeplineHalfEdgePropertyList& propertySet);
	void DebugRender_Scan( NavFloorSweeplineInputElements::Iterator begin, NavFloorSweeplineInputElements::Iterator end );


private:
	SweeplineHalfEdgePropertyList::NodePool m_propertyPool;
	PropertyPool m_propertyListPool;
	KyArray<PropertyKey> m_propertyListKeys; // temp for fast clear() of m_propertyPool

	ScanData m_scanData;
	ScanDataPool m_scanDataPool;

	const KyArray<DynamicNavTag>* m_navTags;
	KyInt32 m_currentX;
	KyInt32 m_justBefore;

	NavFloorSweeplineEndPointQueue m_endpointQueue;
	EnPointPool m_endPointPool;

	SharedPoolList<ScanDataElement> m_insertionSet; //edge to be inserted after current NavFloorSweepline stop
	SharedPoolList<ScanDataElement>::NodePool m_insertionSetPool;

	SharedPoolList<ScanDataElement> m_insertion_elements; //edge to be inserted after current NavFloorSweepline stop
	SharedPoolList<ScanDataElement>::NodePool m_insertion_elementsPool;

	SharedPoolList<MergerRawInputEdgeSubSegment> m_removalSet;
	SharedPoolList<MergerRawInputEdgeSubSegment>::NodePool m_removalSetPool;

	// For debug draw
	KyFloat32 m_rasterPrecision;
	Kaim::Vec3i m_cellOriginOffset; // retrieved from input when Scan() is called. Used for debug Render
	Kaim::ScopedDisplayList* m_displayList;
};

} // namespace Kaim

#endif // Navigation_NavFloorSweepline_H
