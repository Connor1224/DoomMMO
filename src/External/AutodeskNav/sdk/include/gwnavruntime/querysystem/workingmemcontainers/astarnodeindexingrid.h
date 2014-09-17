/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_AstarNodeIndexInGrid_H
#define Navigation_AstarNodeIndexInGrid_H

#include "gwnavruntime/queries/utils/astartypes.h"
#include "gwnavruntime/database/activedata.h"

namespace Kaim
{

class AstarNodeIndexInGrid
{
public:

	KY_INLINE KyUInt32 GetNumberOfWordsForNodeIndices(KyUInt32 numberOfNodes) { return numberOfNodes; }

	/*
		AstarIndex on 32bits
	*/
	struct NavGraphToNodeIndices
	{
		KY_INLINE AStarNodeIndex GetAStarNodeIndex(NavGraphVertexIdx navGraphVertexIdx) const { return GetNodeIndices()[navGraphVertexIdx]; }
		KY_INLINE void SetAStarNodeIndex(NavGraphVertexIdx navGraphVertexIdx, AStarNodeIndex index) { GetNodeIndices()[navGraphVertexIdx] = index; }

		KY_INLINE AStarNodeIndex* GetNodeIndices() const { return (AStarNodeIndex*)((char*)this + m_offsetToNodeIndices); }

		KY_INLINE bool IsValid() const { return m_offsetToNodeIndices != KyUInt32MAXVAL; }
		KyUInt32 m_offsetToNodeIndices;
	};

	/*
		AstarIndex on 32bits
	*/
	struct NavFloorToNodeIndices
	{
		KY_INLINE AStarNodeIndex GetAStarNodeIndex(NavHalfEdgeIdx navHalfEdgeIdx) const { return GetNodeIndices()[navHalfEdgeIdx]; }
		KY_INLINE void SetAStarNodeIndex(NavHalfEdgeIdx navHalfEdgeIdx, AStarNodeIndex index) { GetNodeIndices()[navHalfEdgeIdx] = index; }

		KY_INLINE AStarNodeIndex* GetNodeIndices() const { return (AStarNodeIndex*)((char*)this + m_offsetToNodeIndices); }
		KY_INLINE bool IsValid() const { return m_offsetToNodeIndices != KyUInt32MAXVAL; }

		KyUInt32 m_offsetToNodeIndices;
	};

	struct CellPosToNavFloors
	{
		KY_INLINE bool IsValid() const { return m_offSetToNavFloorToNodeIndices != KyUInt32MAXVAL; }
		KY_INLINE NavFloorToNodeIndices* GetNavFloorToNodeIndices() const { return (NavFloorToNodeIndices*)((char*)this + m_offSetToNavFloorToNodeIndices); }

		KyUInt32 m_offSetToNavFloorToNodeIndices;
		KyUInt32 m_navDataChangeIdx;
	};

public:
	AstarNodeIndexInGrid() : m_numberOfNavGraph(0), m_currentOffsetFromBuffer(0) {}

	AstarNodeIndexInGrid(WorkingMemory* workingMemory, ActiveData* activeData) :
		m_numberOfNavGraph(activeData->GetNavGraphCount()), m_navGraphChangeIdx(activeData->m_navGraphChangeIdx), m_cellBox(activeData->GetCellBox())
	{
		m_workingMemContainerBase.Init(workingMemory);
		m_currentOffsetFromBuffer = 0;
		MakeEmpty();
	}

	void Init(WorkingMemory* workingMemory, ActiveData* activeData)
	{
		m_workingMemContainerBase.Init(workingMemory);
		m_navGraphChangeIdx = activeData->m_navGraphChangeIdx;
		m_numberOfNavGraph = activeData->GetNavGraphCount();
		m_cellBox = activeData->GetCellBox();
		m_currentOffsetFromBuffer = 0;
		MakeEmpty();
	}

	void ReleaseWorkingMemoryBuffer();

	KyUInt32 GetAvailableSizeInBytes() const;

	bool IsEnoughPlaceForAllocation(KyUInt32 sizeInBytes);

	bool TryToResize();

	void MakeEmpty();

	void* AllocateInBufferAndMemsetTo1(KyUInt32 sizeOfOneElementInBytes, KyUInt32 numberOfElements);

	AStarNodeIndex* AllocateAstarNodeIndices(KyUInt32 numberOfedges);

	CellPosToNavFloors* AllocateCellPosToNavFloors(KyUInt32 numberOfCell);
	NavFloorToNodeIndices* AllocateNavFloorToNodeIndex(KyUInt32 numberofFloors);
	NavGraphToNodeIndices* AllocateNavGraphToNodeIndex(KyUInt32 numberOfGraphs);

	CellPosToNavFloors* GetCellPosToNavFloors(const CellPos& cellPos);

	KyResult GetNavFloorToNodeIndices(ActiveData* activeData, const NavFloorRawPtr& navFloorRawPtr, NavFloorToNodeIndices*& nodeIndices);
	KyResult GetNavGraphToNodeIndices(const NavGraphVertexRawPtr& navGraphVertexRawPtr, NavGraphToNodeIndices*& nodeIndices);

	NavGraphToNodeIndices* GetNavGraphToNodeIndices_Unsafe(const NavGraphVertexRawPtr& navGraphVertexRawPtr);
	NavFloorToNodeIndices* GetNavFloorToNodeIndices_Unsafe(const NavFloorRawPtr& navFloorRawPtr);

	bool IsInitialized() const { return m_workingMemContainerBase.IsInitialized(); }
	bool HasNavDataChanged(Database* database);
public:
	WorkingMemContainerBase m_workingMemContainerBase;

	KyUInt32 m_numberOfNavGraph;
	KyUInt32 m_navGraphChangeIdx;

	KyUInt32 m_currentOffsetFromBuffer;
	CellBox m_cellBox;
};

KY_INLINE void AstarNodeIndexInGrid::ReleaseWorkingMemoryBuffer() { m_workingMemContainerBase.ReleaseBuffer(); }

KY_INLINE KyUInt32 AstarNodeIndexInGrid::GetAvailableSizeInBytes() const { return m_workingMemContainerBase.GetBufferSize() - m_currentOffsetFromBuffer; }

KY_INLINE bool AstarNodeIndexInGrid::IsEnoughPlaceForAllocation(KyUInt32 sizeInBytes)
{
	while (GetAvailableSizeInBytes() < sizeInBytes)
	{
		if (TryToResize() == false)
			return false;
	}

	return true;
}

KY_INLINE AStarNodeIndex* AstarNodeIndexInGrid::AllocateAstarNodeIndices(KyUInt32 numberOfedges)
{
	return (AStarNodeIndex*)AllocateInBufferAndMemsetTo1(sizeof(KyUInt32), GetNumberOfWordsForNodeIndices(numberOfedges));
}

KY_INLINE AstarNodeIndexInGrid::NavFloorToNodeIndices* AstarNodeIndexInGrid::AllocateNavFloorToNodeIndex(KyUInt32 numberofFloors)
{
	return (NavFloorToNodeIndices*)AllocateInBufferAndMemsetTo1(sizeof(NavFloorToNodeIndices), numberofFloors);
}

KY_INLINE AstarNodeIndexInGrid::NavGraphToNodeIndices* AstarNodeIndexInGrid::AllocateNavGraphToNodeIndex(KyUInt32 numberOfGraphs)
{
	return (NavGraphToNodeIndices*)AllocateInBufferAndMemsetTo1(sizeof(NavGraphToNodeIndices), numberOfGraphs);
}

KY_INLINE AstarNodeIndexInGrid::CellPosToNavFloors* AstarNodeIndexInGrid::AllocateCellPosToNavFloors(KyUInt32 numberOfCell)
{
	return (CellPosToNavFloors*)AllocateInBufferAndMemsetTo1(sizeof(CellPosToNavFloors), numberOfCell);
}

KY_INLINE AstarNodeIndexInGrid::CellPosToNavFloors* AstarNodeIndexInGrid::GetCellPosToNavFloors(const CellPos& cellPos)
{
	CellPosToNavFloors* memoryStartForGrid = (CellPosToNavFloors*)((NavGraphToNodeIndices*)m_workingMemContainerBase.GetBuffer() + m_numberOfNavGraph);
	return memoryStartForGrid + m_cellBox.GetRowMajorIndex(cellPos);
}

KY_INLINE AstarNodeIndexInGrid::NavGraphToNodeIndices* AstarNodeIndexInGrid::GetNavGraphToNodeIndices_Unsafe(const NavGraphVertexRawPtr& navGraphVertexRawPtr)
{
	NavGraphToNodeIndices* memoryStartForGraphs = (NavGraphToNodeIndices*)m_workingMemContainerBase.GetBuffer();
	return &memoryStartForGraphs[navGraphVertexRawPtr.GetNavGraph()->m_idxInTheActiveDataBuffer];
}

KY_INLINE AstarNodeIndexInGrid::NavFloorToNodeIndices* AstarNodeIndexInGrid::GetNavFloorToNodeIndices_Unsafe(const NavFloorRawPtr& navFloorRawPtr)
{
	KY_DEBUG_ASSERTN(m_cellBox.IsInside(navFloorRawPtr.GetCellPos()), ("Invalid CellBox"));

	NavFloor* navFloor = navFloorRawPtr.GetNavFloor();
	const CellPos& cellPos = navFloor->GetCellPos();

	CellPosToNavFloors* cellPosToNavFloors = GetCellPosToNavFloors(cellPos);
	KY_DEBUG_ASSERTN(cellPosToNavFloors->IsValid(), ("Bad usage of UnSafe function"));

	NavFloorToNodeIndices& navFloorToNodeIndices = cellPosToNavFloors->GetNavFloorToNodeIndices()[navFloor->GetIndexInCollection()];
	KY_DEBUG_ASSERTN(navFloorToNodeIndices.IsValid(), ("Bad usage of UnSafe function"));

	return &navFloorToNodeIndices;
}

}


#endif //Navigation_AstarNodeIndexInGrid_H

