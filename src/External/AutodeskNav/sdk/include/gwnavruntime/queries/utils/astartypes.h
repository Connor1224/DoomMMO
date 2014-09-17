/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_AStarTypes_H
#define Navigation_AStarTypes_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/navmesh/identifiers/navhalfedgeptr.h"
#include "gwnavruntime/navmesh/worldintegerpos.h"

#include "gwnavruntime/navgraph/identifiers/navgraphvertexrawptr.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmembinaryheap.h"

namespace Kaim
{

typedef KyUInt32 AStarNodeIndex;
static const AStarNodeIndex AStarNodeIndex_Invalid = KyUInt32MAXVAL;
static const AStarNodeIndex AStarNodeIndex_StartNode = 0;
static const AStarNodeIndex AStarNodeIndex_DestNode = 1;

typedef KyUInt8 PathNodeType;
static const PathNodeType NodeType_Invalid = KyUInt8MAXVAL;
static const PathNodeType NodeType_NavGraphVertex = 0;
static const PathNodeType NodeType_NavMeshEdge = 1;

class NodeTypeAndRawPtrDataIdx
{
public:
	NodeTypeAndRawPtrDataIdx() : m_data(KyUInt32MAXVAL) {}
	NodeTypeAndRawPtrDataIdx(PathNodeType aStarNodeType, KyUInt32 indexOfRawPtrData) : 
		m_data((indexOfRawPtrData & 0x3FFFFFFF) | (aStarNodeType << 30)) {}

	PathNodeType GetNodeType() const   { return (PathNodeType)(m_data >> 30); }
	KyUInt32 GetIdxOfRawPtrData() const { return m_data & 0x3FFFFFFF; }
	void SetNodeType(PathNodeType nodeType) { m_data = (m_data & 0x3FFFFFFF) | (nodeType << 30); }

	void SetIdxOfRawPtrData(KyUInt32 indexOfRawPtrData) { m_data = (indexOfRawPtrData & 0x3FFFFFFF) | (GetNodeType() << 30); }
private:
	KyUInt32 m_data; // 32 bits : 2 bit for type (see AstarNodeType), 30 bits for Idx
};

class AStarNode
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	AStarNode() :
		m_costFromStart(KyFloat32MAXVAL),
		m_estimatedCostToDest(0.f),
		m_indexOfPredecessor(AStarNodeIndex_Invalid),
		m_indexInBinaryHeap(IndexInBinHeap_UnSet)
		{}

	AStarNode(AStarNodeIndex indexOfPredecessor, const Vec3f& pos, PathNodeType aStarNodeType, KyUInt32 indexOfRawPtrData) :
		m_nodePosition(pos),
		m_costFromStart(KyFloat32MAXVAL),
		m_estimatedCostToDest(0.f),
		m_nodeTypeAndRawPtrDataIdx(aStarNodeType, indexOfRawPtrData),
		m_indexOfPredecessor(indexOfPredecessor),
		m_indexInBinaryHeap(IndexInBinHeap_UnSet)
		{}

	AStarNodeIndex GetIndexOfPredecessor() const { return m_indexOfPredecessor; }
	void SetIndexOfPredecessor(AStarNodeIndex indexOfPredecessor) { m_indexOfPredecessor = indexOfPredecessor; }
	
	PathNodeType GetNodeType()   const { return m_nodeTypeAndRawPtrDataIdx.GetNodeType(); }
	KyUInt32 GetIdxOfRawPtrData() const { return m_nodeTypeAndRawPtrDataIdx.GetIdxOfRawPtrData(); }

	void SetNodeType(PathNodeType nodeType) { m_nodeTypeAndRawPtrDataIdx.SetNodeType(nodeType); }
	void SetIdxOfRawPtrData(KyUInt32 indexOfRawPtrData) { m_nodeTypeAndRawPtrDataIdx.SetIdxOfRawPtrData(indexOfRawPtrData); }

public:
	Vec3f m_nodePosition;
	KyFloat32 m_costFromStart;
	KyFloat32 m_estimatedCostToDest;
	NodeTypeAndRawPtrDataIdx m_nodeTypeAndRawPtrDataIdx; // 32 bits
	AStarNodeIndex m_indexOfPredecessor; // 32bits
	IndexInBinHeap m_indexInBinaryHeap; // 16bits

};

class NavTag;
class AStarCustomizer_Default
{
public:
	KY_INLINE bool      CanTraverse(const NavTag& /*navTag*/)                                                    { return true;                         }
	KY_INLINE KyFloat32 GetGraphEdgeCost(const Vec3f& edgeStart, const Vec3f& edgeEnd, const NavTag& /*navTag*/) { return Distance(edgeStart, edgeEnd); }
	KY_INLINE KyFloat32 GetHeuristicFromDistanceToDest(KyFloat32 distance3DToDestination)                        { return distance3DToDestination;      }
};


class AStarCustomizer_NullHeuristic
{
public:
	KY_INLINE bool      CanTraverse(const NavTag& /*navTag*/)                                                    { return true;                         }
	KY_INLINE KyFloat32 GetGraphEdgeCost(const Vec3f& edgeStart, const Vec3f& edgeEnd, const NavTag& /*navTag*/) { return Distance(edgeStart, edgeEnd); }
	KY_INLINE KyFloat32 GetHeuristicFromDistanceToDest(KyFloat32 /*distance3DToDestination*/)                    { return 0.f;                          }
};

}


#endif //Navigation_AStarTypes_H

