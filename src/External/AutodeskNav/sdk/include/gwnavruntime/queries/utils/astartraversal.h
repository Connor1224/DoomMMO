/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_AStarTraversal_H
#define Navigation_AStarTraversal_H

#include "gwnavruntime/queries/utils/querycommon.h"
#include "gwnavruntime/queries/utils/astartraversalcontext.h"
#include "gwnavruntime/queries/utils/baseastarquery.h"

#include "gwnavruntime/navmesh/navfloortonavgraphlinks.h"
#include "gwnavruntime/navmesh/intersections.h"
#include "gwnavruntime/navmesh/closestpoint.h"

#include "gwnavruntime/database/database.h"

#include "gwnavruntime/basesystem/iperfmarkerinterface.h"

namespace Kaim
{

class QueryUtils;
class ActiveData;

template<class AStarCustomizer>
class AStarTraversal
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Query)
public:

	// Constructs a new instance of this class. 
	AStarTraversal() : m_aStarCustomizer(), m_astarContext(KY_NULL), m_baseAStarQuery(KY_NULL) {}
	AStarTraversal(const AStarCustomizer& customizer) : m_aStarCustomizer(customizer), m_astarContext(KY_NULL), m_baseAStarQuery(KY_NULL) {}

	// Recovers all inputs so that the query can be advanced. 
	KyResult InitializeContextAndStartAndDestNode(QueryUtils& queryUtils, const Vec3f& startPos3f, const Vec3f& destPos3f,
		const NavTrianglePtr& startTrianglePtr, const NavTrianglePtr& destTrianglePtr);

	KY_INLINE bool IsThereNodeToVisit() { return m_astarContext->m_traversalBinHeap.IsEmpty() == false; }
	KY_INLINE bool HasExplorationReachedDestNode() { return m_astarContext->m_aStarNodes[AStarNodeIndex_DestNode].m_indexOfPredecessor != AStarNodeIndex_Invalid; }

	// Performs a single iteration of the AStar Algorithm. 
	KyResult VisitNode(QueryUtils& queryUtils, bool& doesPathMaxDistReached);

	void DisplayListPropagation(Database* database);
private:
	KyResult ExploreNeighboursOfStartNode(ActiveData* activeData, const NavTriangleRawPtr& startTriangleRawPtr);

	KyResult ExploreNeihgboursOfHalfEdgeNode(ActiveData* activeData, AStarNodeIndex indexOfOpenWithLowerCost, const NavHalfEdgeRawPtr& halfEdgeRawPtr);
	KyResult ExploreNeihgboursOfGraphVertexNode(ActiveData* activeData, AStarNodeIndex indexOfOpenWithLowerCost);

	KyResult ExploreHalfEdgesOfTriangle(ActiveData* activeData, const NavTriangleRawPtr& triangleRawPtr, AStarNodeIndex currentNodeIndex);
	KyResult ExploreGraphVerticesInTriangle(ActiveData* activeData, const NavTriangleRawPtr& triangleRawPtr, AStarNodeIndex currentNodeIndex);

	KyResult CreateNewHalfEdgeNode(ActiveData* activeData, const NavHalfEdgeRawPtr& HalfEdge, const NavHalfEdgeRawPtr& pairHalfEdge,
		const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, AStarNodeIndex predecessorIndex);

	KyResult CreateNewGraphVertexNode(const NavGraphVertexRawPtr& navGraphVertexRawPtr, const NavTag& navTag, AStarNodeIndex predecessorIndex);

	KyResult UpdateDestNode(const NavTag& navTag, AStarNodeIndex currentNodeIndex);

	inline KyResult OpenOrUpdateGraphVertex(ActiveData* activeData, const NavGraphVertexRawPtr& navGraphVertexRawPtr, const NavTag& navTag,
		AStarNodeIndex vertexNodeIndex, AStarNodeIndex currentNodeIndex);

	inline KyResult OpenOrUpdateHalfEdge(ActiveData* activeData, const NavHalfEdgeRawPtr& halfEdge, AStarNodeIndex halfEdgeNodeIndex,
		const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, const NavTag& navTag, AStarNodeIndex currentNodeIndex);

	KyResult UpdateOpenedOrClosedNode(const NavTag& navTag, AStarNodeIndex nodeIndex, AStarNodeIndex predecessorIndex);

	bool ShouldOpenHalfEdgeNode(ActiveData* activeData, const NavHalfEdgeRawPtr& halfEdge, NavHalfEdgeRawPtr& pairHalfEdgeOfNewNode);
	bool ShouldOpenGraphVertexNode(ActiveData* activeData, const NavGraphVertexRawPtr& navGraphVertexRawPtr);

	KyResult ComputeNodeCost(AStarNode* currentNode, AStarNode* nextNode, AStarNode* prevNode);

	void ComputeNodePositionOnHalfEdgeAndCreateAstarNode(const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, AStarNodeIndex predecessorNodeIndex);

	bool CanTraverse(const NavTag& navTag) { return m_aStarCustomizer.CanTraverse(navTag); }

	KyFloat32 EvaluateEdgeCost(const AStarNode& node1, const AStarNode& node2, const NavTag& navTag); // Check node type to call EvaluateEdgeCostOnNavMesh or EvaluateGraphEdgeCost

	KyFloat32 EvaluateEdgeCostOnNavMesh(const AStarNode& node1, const AStarNode& node2) { return Distance(node1.m_nodePosition, node2.m_nodePosition); }
	KyFloat32 EvaluateGraphEdgeCost(const AStarNode& node1, const AStarNode& node2, const NavTag& navTag)
	{
		return m_aStarCustomizer.GetGraphEdgeCost(node1.m_nodePosition, node2.m_nodePosition, navTag);
	}
	KyFloat32 EvaluateCostToDest(const AStarNode& node1)
	{
		return m_aStarCustomizer.GetHeuristicFromDistanceToDest(Distance(node1.m_nodePosition, m_astarContext->m_aStarNodes[AStarNodeIndex_DestNode].m_nodePosition));
	}
public:
	AStarCustomizer m_aStarCustomizer;
public:
	AStarTraversalContext* m_astarContext;
	NavTriangleRawPtr m_destTriangleRawPtr;
	BaseAStarQuery* m_baseAStarQuery;
};

#include "gwnavruntime/queries/utils/astartraversal.inl"

}

#endif //Navigation_AStarTraversal_H

