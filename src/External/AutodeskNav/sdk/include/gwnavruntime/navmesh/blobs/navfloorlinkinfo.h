/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: LASI
#ifndef Navigation_NavFloorLinkInfo_H
#define Navigation_NavFloorLinkInfo_H

#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/blob/blobarray.h"

namespace Kaim
{

class NavFloorLinkInfo
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavFloorLinkInfo()
	{
		m_firstIdxOfHalfEdgeForType[0] = 0;
		m_firstIdxOfHalfEdgeForType[1] = 0;
		m_firstIdxOfHalfEdgeForType[2] = 0;
		m_firstIdxOfHalfEdgeForType[3] = 0;
		m_firstIdxOfHalfEdgeForType[4] = 0;
		m_halfEdgeCountForType[0] = 0;
		m_halfEdgeCountForType[1] = 0;
		m_halfEdgeCountForType[2] = 0;
		m_halfEdgeCountForType[3] = 0;
		m_halfEdgeCountForType[4] = 0;
	}
	// store the indices of edges of type
	// EDGETYPE_CELLBOUNDARY_EAST, EDGETYPE_CELLBOUNDARY_NORTH, EDGETYPE_CELLBOUNDARY_WEST , EDGETYPE_CELLBOUNDARY_SOUTH, EDGETYPE_FLOORBOUNDARY
	// Indices are stored in this order

	KyUInt16 m_firstIdxOfHalfEdgeForType[5]; // see NavHalfEdgeType
	KyUInt16 m_halfEdgeCountForType[5]; // see NavHalfEdgeType
	BlobArray<CompactNavHalfEdgeIdx> m_navHalfEdgeLinkIndices;
};
KY_INLINE void SwapEndianness(Endianness::Target e, NavFloorLinkInfo& self)
{
	SwapEndianness(e, self.m_firstIdxOfHalfEdgeForType[0]);
	SwapEndianness(e, self.m_firstIdxOfHalfEdgeForType[1]);
	SwapEndianness(e, self.m_firstIdxOfHalfEdgeForType[2]);
	SwapEndianness(e, self.m_firstIdxOfHalfEdgeForType[3]);
	SwapEndianness(e, self.m_firstIdxOfHalfEdgeForType[4]);
	SwapEndianness(e, self.m_halfEdgeCountForType[0]);
	SwapEndianness(e, self.m_halfEdgeCountForType[1]);
	SwapEndianness(e, self.m_halfEdgeCountForType[2]);
	SwapEndianness(e, self.m_halfEdgeCountForType[3]);
	SwapEndianness(e, self.m_halfEdgeCountForType[4]);
	SwapEndianness(e, self.m_navHalfEdgeLinkIndices);
}

}

#endif //Navigation_NavFloorLinkInfo_H


