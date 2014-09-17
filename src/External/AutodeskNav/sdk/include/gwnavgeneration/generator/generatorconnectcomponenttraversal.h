/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_GeneratorConnectComponentTraversal_H
#define Navigation_GeneratorConnectComponentTraversal_H

#include "gwnavruntime/base/memory.h"

#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/navmesh/identifiers/navhalfedgerawptr.h"

#include "gwnavgeneration/generator/generatornavdatafilter.h"

namespace Kaim
{

class NavMeshElementBlob;

// Implementation detail of PdgNavDataFilter
class GeneratorConnectComponentTraversal
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public :
	GeneratorConnectComponentTraversal() :
		m_openNodesPool(Memory::pGlobalHeap, MemStat_NavDataGen, PoolChunkSize::SlotCount, 64),
		m_openNodes(&m_openNodesPool)
	{}

public :
	void Clear() { m_openNodes.Clear(); }

	void Search();

	KY_INLINE void AddStartFloor(const NavFloorRawPtr& navFloorRawPtr)
	{
		navFloorRawPtr.GetNavFloor()->SetConnectedComponentId(m_currentComponentId);
		m_openNodes.PushBack(navFloorRawPtr);
	}

	KY_INLINE void SetCurrentComponentId(ConnectedComponentId currentComponentId)
	{
		m_currentComponentId = currentComponentId;
	}


private:

	KY_INLINE void OpenNodeIfNew(const NavFloorRawPtr& navFloorRawPtr)
	{
		ConnectedComponentId connectedComponentId = navFloorRawPtr.GetNavFloor()->GetConnectedComponentId();

		if (connectedComponentId == ConnectedComponent_UNDEFINED)
		{
			navFloorRawPtr.GetNavFloor()->SetConnectedComponentId(m_currentComponentId);
			m_openNodes.PushBack(navFloorRawPtr);
		}
	}

public:
	SharedPoolList<NavFloorRawPtr>::NodePool m_openNodesPool;
	SharedPoolList<NavFloorRawPtr> m_openNodes;
	ConnectedComponentId m_currentComponentId;
};


inline void GeneratorConnectComponentTraversal::Search()
{
	NavFloorRawPtr currentFloorRawPtr;
	SharedPoolList<NavFloorRawPtr>::Iterator firstIterator = m_openNodes.GetFirst();
	while (m_openNodes.IsEmpty() == false)
	{
		currentFloorRawPtr = *firstIterator;
		m_openNodes.Erase(firstIterator);

		NavFloor& navFloor = *currentFloorRawPtr.GetNavFloor();
		const KyUInt32 linkCount = navFloor.m_numberOfLink;
		NavHalfEdgeRawPtr* links = navFloor.GetLinksBuffer();

		for(KyUInt32 linkIdx = 0; linkIdx < linkCount; ++linkIdx)
		{
			if (links[linkIdx].IsValid())
				OpenNodeIfNew(links[linkIdx].m_navFloorRawPtr);
		}

		firstIterator = m_openNodes.GetFirst();
	}
}



}

#endif // Navigation_GeneratorConnectComponentTraversal_H

