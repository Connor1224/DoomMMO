/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: JUBA - secondary contact: NOBODY

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::InitializeContextAndStartAndDestNode(QueryUtils& queryUtils, const Vec3f& startPos3f, const Vec3f& destPos3f,
	const NavTrianglePtr& startTrianglePtr, const NavTrianglePtr& destTrianglePtr)
{
	KY_ASSERT(startTrianglePtr.IsValid());
	KY_ASSERT(destTrianglePtr.IsValid());

	KY_FORWARD_ERROR_NO_LOG(m_astarContext->Init(queryUtils));

	const NavTriangleRawPtr startTriangleRawPtr = startTrianglePtr.GetRawPtr();
	m_destTriangleRawPtr = destTrianglePtr.GetRawPtr();

	// set the first edge of start triangle for startPoint
	const NavHalfEdgeRawPtr startEdgeRawPtr(startTriangleRawPtr.m_navFloorRawPtr, NavFloorBlob::NavTriangleIdxToFirstNavHalfEdgeIdx(startTriangleRawPtr.GetTriangleIdx()));
	// set the first edge of start triangle for startPoint
	const NavHalfEdgeRawPtr destEdgeRawPtr(m_destTriangleRawPtr.m_navFloorRawPtr, NavFloorBlob::NavTriangleIdxToFirstNavHalfEdgeIdx(m_destTriangleRawPtr.GetTriangleIdx()));

	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;
	WorkingMemArray<NavHalfEdgeRawPtr>& aStarNodesEdgeRawPtrs = m_astarContext->m_edgeRawPtrNodes;

	//create a node for the start point
	aStarNodes.PushBack(AStarNode(AStarNodeIndex_Invalid, startPos3f, NodeType_NavMeshEdge, 0));
	KY_ASSERT(AStarNodeIndex_StartNode == aStarNodes.GetCount() - 1);
	KY_FORWARD_ERROR_NO_LOG(aStarNodesEdgeRawPtrs.PushBack(startEdgeRawPtr));

	//create a node for the destination point
	aStarNodes.PushBack(AStarNode(AStarNodeIndex_Invalid, destPos3f, NodeType_NavMeshEdge, 1));
	KY_ASSERT(AStarNodeIndex_DestNode == aStarNodes.GetCount() - 1);
	KY_FORWARD_ERROR_NO_LOG(aStarNodesEdgeRawPtrs.PushBack(destEdgeRawPtr));
	
	//Init the startNode values
	aStarNodes[AStarNodeIndex_StartNode].m_costFromStart = 0.f;
	aStarNodes[AStarNodeIndex_StartNode].m_estimatedCostToDest = EvaluateCostToDest(aStarNodes[AStarNodeIndex_StartNode]);

	// init the traversal with the neighbours of the startNode
	KY_FORWARD_ERROR_NO_LOG(ExploreNeighboursOfStartNode(queryUtils.m_database->GetActiveData(), startTriangleRawPtr));

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::VisitNode(QueryUtils& queryUtils, bool& doesPathMaxDistReached)
{
	ActiveData* activeData = queryUtils.m_database->GetActiveData();
	doesPathMaxDistReached = false;
	KY_UNUSED(activeData);

	// retrieve the node with the lower cost from the binary heap
	AStarNodeIndex lowerCostNodeIndex = AStarNodeIndex_Invalid;
	m_astarContext->m_traversalBinHeap.ExtractFirst(lowerCostNodeIndex);

	// check the behavior of the binaryHeap
	KY_ASSERT(m_astarContext->m_aStarNodes[lowerCostNodeIndex].m_indexInBinaryHeap == IndexInBinHeap_Removed);

	// check if we do not reach the destNode
	if (lowerCostNodeIndex == AStarNodeIndex_DestNode)
	{
		// empty the binary
		m_astarContext->m_traversalBinHeap.Clear();
		return KY_SUCCESS;
	}

	// we never put the startNode in the BinHeap
	KY_ASSERT(lowerCostNodeIndex != AStarNodeIndex_StartNode);

	// We do not reach the dest node, go through the neighbours of the node
	AStarNode& lowerCostNode = m_astarContext->m_aStarNodes[lowerCostNodeIndex];

	if (lowerCostNode.m_costFromStart + lowerCostNode.m_estimatedCostToDest >= m_baseAStarQuery->GetPathMaxCost())
	{
		doesPathMaxDistReached = true;
		return KY_ERROR;
	}

	Kaim::PathNodeType pathNodeType = lowerCostNode.GetNodeType();
	switch (pathNodeType)
	{
	case NodeType_NavMeshEdge :
		{
			// retrieve the corresponding NavHalfEdgeRawPtr and its pair NavHalfEdgeRawPtr
			NavHalfEdgeRawPtr currentEdgeRawPtr = m_astarContext->m_edgeRawPtrNodes[lowerCostNode.GetIdxOfRawPtrData()];
			NavHalfEdgeRawPtr pairHalfEdgeRawPtr;

			const bool isHalfEdgeCrossable = currentEdgeRawPtr.IsHalfEdgeCrossable<AStarCustomizer>(pairHalfEdgeRawPtr, &m_aStarCustomizer);
			KY_UNUSED(isHalfEdgeCrossable);

		#if defined(KY_BUILD_DEBUG)
			KY_ASSERT(isHalfEdgeCrossable); // no node should have been created for a non crossable edge
			// the current and its paired edge must have the same AStarNodeIndex
			AstarNodeIndexInGrid::NavFloorToNodeIndices* navFloorToNodeIndices = m_astarContext->m_edgeIndexGrid.GetNavFloorToNodeIndices_Unsafe(pairHalfEdgeRawPtr.m_navFloorRawPtr);
			KY_ASSERT(navFloorToNodeIndices->GetAStarNodeIndex(pairHalfEdgeRawPtr.GetHalfEdgeIdx()) == lowerCostNodeIndex);
		#endif

			// explore neighbours of edge and pair edge
			KY_FORWARD_ERROR_NO_LOG(ExploreNeihgboursOfHalfEdgeNode(activeData, lowerCostNodeIndex, currentEdgeRawPtr));
			KY_FORWARD_ERROR_NO_LOG(ExploreNeihgboursOfHalfEdgeNode(activeData, lowerCostNodeIndex, pairHalfEdgeRawPtr));
			break;
		}
	case NodeType_NavGraphVertex :
		{
			KY_FORWARD_ERROR_NO_LOG(ExploreNeihgboursOfGraphVertexNode(activeData, lowerCostNodeIndex));
			break;
		}
	default:
		{
			KY_ASSERT(false);
			break;
		}
	}

	// node visited !
	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::ExploreNeighboursOfStartNode(ActiveData* activeData, const NavTriangleRawPtr& startTriangleRawPtr)
{
	KY_FORWARD_ERROR_NO_LOG(ExploreHalfEdgesOfTriangle(activeData, startTriangleRawPtr, AStarNodeIndex_StartNode));
	KY_FORWARD_ERROR_NO_LOG(ExploreGraphVerticesInTriangle(activeData, startTriangleRawPtr, AStarNodeIndex_StartNode));

	if (startTriangleRawPtr == m_destTriangleRawPtr)
	{
		KY_FORWARD_ERROR_NO_LOG(UpdateDestNode(m_destTriangleRawPtr.GetNavTag(), AStarNodeIndex_StartNode));
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::ExploreNeihgboursOfHalfEdgeNode(ActiveData* activeData, AStarNodeIndex currentNodeIndex,
	const NavHalfEdgeRawPtr& currentEdgeRawPtr)
{
	AstarNodeIndexInGrid& edgeIndexGrid = m_astarContext->m_edgeIndexGrid;
	Vec3f vertices[3];

	AstarNodeIndexInGrid::NavFloorToNodeIndices* const navFloorToNodeIndices = edgeIndexGrid.GetNavFloorToNodeIndices_Unsafe(currentEdgeRawPtr.m_navFloorRawPtr);

	const NavTriangleRawPtr triangleRawPtr    (currentEdgeRawPtr.m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(currentEdgeRawPtr.GetHalfEdgeIdx()));
	const NavHalfEdgeRawPtr nextHalfEdgeRawPtr(currentEdgeRawPtr.m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToNextNavHalfEdgeIdx(currentEdgeRawPtr.GetHalfEdgeIdx()));
	const NavHalfEdgeRawPtr prevHalfEdgeRawPtr(currentEdgeRawPtr.m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToPrevNavHalfEdgeIdx(currentEdgeRawPtr.GetHalfEdgeIdx()));

	AStarNodeIndex nextNodeIndex = navFloorToNodeIndices->GetAStarNodeIndex(nextHalfEdgeRawPtr.GetHalfEdgeIdx());
	AStarNodeIndex prevNodeIndex = navFloorToNodeIndices->GetAStarNodeIndex(prevHalfEdgeRawPtr.GetHalfEdgeIdx());

	// compute the indices of the edges in the triangle (0, 1 or 2) to identify the start and end vertex of each edge
	const KyUInt32 currentIdxInTriangle = NavFloorBlob::NavHalfEdgeIdxToHalfEdgeNumberInTriangle( currentEdgeRawPtr.GetHalfEdgeIdx());
	const KyUInt32 nextIdxInTriangle    = NavFloorBlob::NavHalfEdgeIdxToHalfEdgeNumberInTriangle(nextHalfEdgeRawPtr.GetHalfEdgeIdx());
	const KyUInt32 prevIdxInTriangle    = NavFloorBlob::NavHalfEdgeIdxToHalfEdgeNumberInTriangle(prevHalfEdgeRawPtr.GetHalfEdgeIdx());

	if (nextNodeIndex == AStarNodeIndex_Invalid || prevNodeIndex == AStarNodeIndex_Invalid)
		triangleRawPtr.GetVerticesPos3f(vertices[0], vertices[1], vertices[2]);

	const NavTag& navTag = currentEdgeRawPtr.GetNavTag();
	KY_FORWARD_ERROR_NO_LOG(OpenOrUpdateHalfEdge(activeData, nextHalfEdgeRawPtr, nextNodeIndex, vertices[nextIdxInTriangle], vertices[prevIdxInTriangle], navTag, currentNodeIndex));
	KY_FORWARD_ERROR_NO_LOG(OpenOrUpdateHalfEdge(activeData, prevHalfEdgeRawPtr, prevNodeIndex, vertices[prevIdxInTriangle], vertices[currentIdxInTriangle], navTag, currentNodeIndex));
	KY_FORWARD_ERROR_NO_LOG(ExploreGraphVerticesInTriangle(activeData, triangleRawPtr, currentNodeIndex));

	if (triangleRawPtr == m_destTriangleRawPtr)
	{
		KY_FORWARD_ERROR_NO_LOG(UpdateDestNode(navTag, currentNodeIndex));// check if we do not have to update the DestNode
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::ExploreNeihgboursOfGraphVertexNode(ActiveData* activeData, AStarNodeIndex currentNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes   = m_astarContext->m_aStarNodes;
	AstarNodeIndexInGrid& edgeIndexGrid = m_astarContext->m_edgeIndexGrid;

	const KyUInt32 indexOfvertexRawPtr = aStarNodes[currentNodeIndex].GetIdxOfRawPtrData();

	// this variable can be use till a create a new node because it may cause a memory reallocation which will cause the reference to be invalid
	// that is why we store a copy of the NavGraphRawPtr and the VertexIdx
	NavGraphVertexRawPtr& navGraphVertexRawPtr = m_astarContext->m_vertexRawPtrNodes[indexOfvertexRawPtr];
	const NavGraphRawPtr navGraphRawPtr        = navGraphVertexRawPtr.m_navGraphRawPtr;
	const NavGraphVertexIdx navGraphVertexIdx  = navGraphVertexRawPtr.GetNavGraphVertexIdx();

	// we are processing neighbours of a node that exists, so memory for indices of its navGraph should have been allocated yet -> unsafe function
	// WE SHOULD NOT RESIZE THE GRID WHEN BROWSING THE VERTICES OF THE SAME GRAPH
	AstarNodeIndexInGrid::NavGraphToNodeIndices* const navGraphToNodeIndices = edgeIndexGrid.GetNavGraphToNodeIndices_Unsafe(navGraphVertexRawPtr);

	const NavGraphBlob& navGraphBlob = *navGraphRawPtr.GetNavGraphBlob();

	const NavGraphVertex& navGraphVertex = navGraphBlob.GetNavGraphVertex(navGraphVertexIdx);
	const KyUInt32 neighbourCount        = navGraphVertex.GetNeihbourVertexCount();

	// browse the graphVertex neighbour nodes
	for(KyUInt32 i = 0; i < neighbourCount; ++i)
	{
		const NavGraphVertexIdx& neighbourVertexIdx = navGraphVertex.GetNeihbourVertexIdx(i);
		NavTagIdx edgeNavTagIdx =  navGraphVertex.GetNavGraphEdgeNavTagIdx(i);
		const NavTag& navTag = navGraphBlob.GetNavTag(edgeNavTagIdx);

		if (CanTraverse(navTag) == false)
			continue;

		const AStarNodeIndex neighbourNodeIndex = navGraphToNodeIndices->GetAStarNodeIndex(neighbourVertexIdx);

		KY_FORWARD_ERROR_NO_LOG(OpenOrUpdateGraphVertex(activeData, NavGraphVertexRawPtr(navGraphRawPtr, neighbourVertexIdx), navTag,
			neighbourNodeIndex, currentNodeIndex));
	}


	// Check if this graphvertex is connected to a triangle node
	const GraphVertexData& vertexData = navGraphRawPtr.GetNavGraph()->GetGraphVertexData(navGraphVertexIdx);
	NavGraphLink* navGraphLink = vertexData.m_navGraphLink;

	if (navGraphLink == KY_NULL || navGraphLink->IsValid() == false)
		// vertex is not linked to the navMesh
		return KY_SUCCESS;

	const NavTriangleRawPtr& triangleRawPtr = navGraphLink->m_navTriangleRawPtr;
	const NavTag& triangleNavTag = triangleRawPtr.GetNavTag();

	// TODO : REMOVE THAT !
	if (CanTraverse(triangleNavTag) == false)
		return KY_SUCCESS;

	KY_FORWARD_ERROR_NO_LOG(ExploreHalfEdgesOfTriangle(activeData, triangleRawPtr, currentNodeIndex));
	KY_FORWARD_ERROR_NO_LOG(ExploreGraphVerticesInTriangle(activeData, triangleRawPtr, currentNodeIndex));

	if (triangleRawPtr == m_destTriangleRawPtr)
	{
		KY_FORWARD_ERROR_NO_LOG(UpdateDestNode(triangleNavTag, currentNodeIndex));
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
void AStarTraversal<AStarCustomizer>::ComputeNodePositionOnHalfEdgeAndCreateAstarNode(
	const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, AStarNodeIndex predecessorNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;
	WorkingMemArray<NavHalfEdgeRawPtr>& aStarNodesEdgeRawPtrs = m_astarContext->m_edgeRawPtrNodes;
/*
	aStarNodes.PushBack_UnSafe(AStarNode(predecessorNodeIndex, (endPosOfEdge + startPosOfEdge) * 0.5f, NodeType_NavMeshEdge, aStarNodesEdgeRawPtrs.GetCount()));
	return;*/

	AStarNode& predecessorNode = aStarNodes[predecessorNodeIndex];
	Vec3f nodePos, grandPredecessorNodePos;
	const Vec3f edge(endPosOfEdge - startPosOfEdge);
	Vec3f reducedStartPosOfEdge;
	Vec3f reducedendPosOfEdge;
	const KyFloat32 edgeSquareLength = edge.GetSquareLength();
	// Do not move more than 40cm from the wall, so the limit if 10% = 30cm -> 3m
	const KyFloat32 maxDistFromWall = 0.3f;
	const KyFloat32 maxSquareDistEdge = 0.3f * 10.f * 0.3f * 10.f;
	if (edgeSquareLength > maxSquareDistEdge)
	{
		const KyFloat32 length = sqrtf(edgeSquareLength);
		const Vec3f dir = edge / length;
		reducedStartPosOfEdge = startPosOfEdge + dir * maxDistFromWall;
		reducedendPosOfEdge= endPosOfEdge - dir * maxDistFromWall;
	}
	else
	{
		reducedStartPosOfEdge = startPosOfEdge + edge * 0.1f; // 10% of margin to not be as far enough from walls
		reducedendPosOfEdge= endPosOfEdge - edge * 0.1f;
	}

	const Vec3f& destPos = m_astarContext->m_aStarNodes[AStarNodeIndex_DestNode].m_nodePosition;

	bool canTryToGoStraigthForward = predecessorNodeIndex != AStarNodeIndex_StartNode &&
		predecessorNode.GetNodeType() != NodeType_NavGraphVertex &&
		predecessorNode.GetNodeType() != NodeType_NavMeshEdge;

	if (canTryToGoStraigthForward)
		grandPredecessorNodePos = aStarNodes[predecessorNode.GetIndexOfPredecessor()].m_nodePosition;

	if (canTryToGoStraigthForward &&
		Intersections::SegmentVsSegment2d(reducedStartPosOfEdge, reducedendPosOfEdge, grandPredecessorNodePos,
		grandPredecessorNodePos + (predecessorNode.m_nodePosition - grandPredecessorNodePos) * 1000.f, nodePos))
	{
		// previous node is not a GraphVertexNode and we can keep on going straigth forward
		aStarNodes.PushBack_UnSafe(AStarNode(/*predecessorNode.GetIndexOfPredecessor()*/predecessorNodeIndex, nodePos, NodeType_NavMeshEdge, aStarNodesEdgeRawPtrs.GetCount()));
	}
	else
	{
		// we cannot keep on going straigth forward
		// try to find a position on the go straigth towards the dest pos
		if (Intersections::SegmentVsSegment2d(reducedStartPosOfEdge, reducedendPosOfEdge, predecessorNode.m_nodePosition, destPos, nodePos))
		{
			// Ok find a new position
			// insert node in the binary Heap
			aStarNodes.PushBack_UnSafe(AStarNode(predecessorNodeIndex, nodePos, NodeType_NavMeshEdge, aStarNodesEdgeRawPtrs.GetCount()));
		}
		else
		{
			// no "good" position on the edge, try to find a position towards destPos
			KyFloat32 squareDist;
			ClosestPoint::OnSegmentVsPoint2d(reducedStartPosOfEdge, reducedendPosOfEdge, destPos, nodePos, squareDist);
			// insert node in the binary Heap
			aStarNodes.PushBack_UnSafe(AStarNode(predecessorNodeIndex, nodePos, NodeType_NavMeshEdge, aStarNodesEdgeRawPtrs.GetCount()));
		}
	}
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::CreateNewHalfEdgeNode(ActiveData* activeData,
	const NavHalfEdgeRawPtr& halfEdge, const NavHalfEdgeRawPtr& pairHalfEdge,
	const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, AStarNodeIndex predecessorNodeIndex)
{
	
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckAstarNodeArrayMemory()); // check for memory to create a new node
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckNavHalfEdgeRawPtrArrayMemory()); // check for memory to create a new node
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckTraversalBinaryHeapMemory()); // check for memory to insert a newElement in the binary heap

	WorkingMemArray<AStarNode>& aStarNodes                    = m_astarContext->m_aStarNodes;
	WorkingMemArray<NavHalfEdgeRawPtr>& aStarNodesEdgeRawPtrs = m_astarContext->m_edgeRawPtrNodes;
	AstarNodeIndexInGrid& edgeIndexGrid                       = m_astarContext->m_edgeIndexGrid;

	// the new node Index
	AStarNodeIndex newNodeIndex = (AStarNodeIndex)aStarNodes.GetCount();

	ComputeNodePositionOnHalfEdgeAndCreateAstarNode(startPosOfEdge, endPosOfEdge, predecessorNodeIndex);
	aStarNodesEdgeRawPtrs.PushBack(halfEdge);

	AstarNodeIndexInGrid::NavFloorToNodeIndices* navFloorToNodeIndices = edgeIndexGrid.GetNavFloorToNodeIndices_Unsafe(halfEdge.m_navFloorRawPtr);
	// set the index of the current edge
	navFloorToNodeIndices->SetAStarNodeIndex(halfEdge.GetHalfEdgeIdx(), newNodeIndex);

	if (halfEdge.m_navFloorRawPtr != pairHalfEdge.m_navFloorRawPtr)
	{
		// retrieve node indices of the navfloor that contains the pair halfEdge
		if (KY_FAILED(edgeIndexGrid.GetNavFloorToNodeIndices(activeData, pairHalfEdge.m_navFloorRawPtr, navFloorToNodeIndices)))
		{
			KY_LOG_WARNING( ("This traversal reached the maximum size of propagation memory in the activeData"));
			return KY_ERROR;
		}
	}
	// set the index to the pair edge
	navFloorToNodeIndices->SetAStarNodeIndex(pairHalfEdge.GetHalfEdgeIdx(), newNodeIndex);

	AStarNode& newNode = aStarNodes[newNodeIndex];

	// compute the estimated cost to dest once and for all
	newNode.m_estimatedCostToDest = EvaluateCostToDest(newNode);

	// compute first cost of the node
	AStarNode& predecessorNodeReload = aStarNodes[predecessorNodeIndex]; // reload because of a possible resize of the astarNodeArray
	newNode.m_costFromStart = predecessorNodeReload.m_costFromStart + EvaluateEdgeCostOnNavMesh(predecessorNodeReload, newNode);
	m_astarContext->m_traversalBinHeap.Insert(newNodeIndex);

	return KY_SUCCESS;
}


template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::CreateNewGraphVertexNode(const NavGraphVertexRawPtr& navGraphVertexRawPtr, const NavTag& navTag, AStarNodeIndex predecessorNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;
	WorkingMemArray<NavGraphVertexRawPtr>& aStarNodesVertexRawPtrs = m_astarContext->m_vertexRawPtrNodes;

	// check for memory to create a new node
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckAstarNodeArrayMemory());
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckNavGraphVertexRawPtrArrayMemory());
	// check for memory to insert a newElement in the binary heap
	KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckTraversalBinaryHeapMemory());

	NavGraph* navGraph = navGraphVertexRawPtr.GetNavGraph();
	const NavGraphBlob& navGraphBlob = *navGraph->GetNavGraphBlob();

	const Vec3f& vertexPosition = navGraphBlob.GetNavGraphVertexPosition(navGraphVertexRawPtr.GetNavGraphVertexIdx());

	// set the new node Index
	AStarNodeIndex newNodeIndex = (AStarNodeIndex)aStarNodes.GetCount();

	// insert node in the binary Heap
	aStarNodes.PushBack(AStarNode(predecessorNodeIndex, vertexPosition, NodeType_NavGraphVertex, aStarNodesVertexRawPtrs.GetCount()));
	aStarNodesVertexRawPtrs.PushBack(navGraphVertexRawPtr);

	AstarNodeIndexInGrid::NavGraphToNodeIndices* navGraphToNodeIndices = m_astarContext->m_edgeIndexGrid.GetNavGraphToNodeIndices_Unsafe(navGraphVertexRawPtr);
	navGraphToNodeIndices->SetAStarNodeIndex(navGraphVertexRawPtr.GetNavGraphVertexIdx(), newNodeIndex);

	AStarNode& newNode = aStarNodes[newNodeIndex];

	// compute the estimated cost to dest once and for all
	newNode.m_estimatedCostToDest = EvaluateCostToDest(newNode);

	// compute first cost of the node
	AStarNode& predecessorNode = aStarNodes[predecessorNodeIndex];
	if (predecessorNode.GetNodeType() == NodeType_NavGraphVertex)
	{
		const KyUInt32 indexOfvertexRawPtr = predecessorNode.GetIdxOfRawPtrData();
		NavGraphVertexRawPtr& predecessorNavGraphVertexRawPtr = m_astarContext->m_vertexRawPtrNodes[indexOfvertexRawPtr];
		if (predecessorNavGraphVertexRawPtr.GetNavGraph() == navGraphVertexRawPtr.GetNavGraph())
		{
			// both node are GraphVertex and on the same graph
			newNode.m_costFromStart = predecessorNode.m_costFromStart + EvaluateGraphEdgeCost(predecessorNode, newNode, navTag);
		}
		else
		{
			newNode.m_costFromStart = predecessorNode.m_costFromStart + EvaluateEdgeCostOnNavMesh(predecessorNode, newNode);
		}
	}
	else
	{
		newNode.m_costFromStart = predecessorNode.m_costFromStart + EvaluateEdgeCostOnNavMesh(predecessorNode, newNode);
	}

	m_astarContext->m_traversalBinHeap.Insert(newNodeIndex);

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::ExploreGraphVerticesInTriangle(ActiveData* activeData, const NavTriangleRawPtr& triangleRawPtr,
	AStarNodeIndex currentNodeIndex)
{
	AstarNodeIndexInGrid& edgeIndexGrid = m_astarContext->m_edgeIndexGrid;

	NavFloor* navFloor = triangleRawPtr.GetNavFloor();
	NavGraphLinkCollection& navGraphLinks = navFloor->GetNavFloorToNavGraphLinks()->m_navGraphLinks;
	const NavTag& navTag = triangleRawPtr.GetNavTag();

	for (KyUInt32 linkIdx = 0 ; linkIdx < navGraphLinks.GetCount(); ++linkIdx)
	{
		NavGraphLink* navGraphLink = navGraphLinks[linkIdx];
		if (triangleRawPtr.GetTriangleIdx() != navGraphLink->m_navTriangleRawPtr.GetTriangleIdx())
			continue;

		const NavGraphVertexRawPtr& navGraphVertexRawPtr = navGraphLink->m_navGraphVertexRawPtr;

		// declare navGraphToNodeIndices in a local scope to be sure it won't be used after a memory resize
		AstarNodeIndexInGrid::NavGraphToNodeIndices* navGraphToNodeIndices = KY_NULL;
		if (KY_FAILED(edgeIndexGrid.GetNavGraphToNodeIndices(navGraphVertexRawPtr, navGraphToNodeIndices)))
		{
			KY_LOG_WARNING( ("No more memory to store the AstarNodeIndex of a new encountered NavGraph"));
			return KY_ERROR;
		}

		const AStarNodeIndex vertexNodeIndex = navGraphToNodeIndices->GetAStarNodeIndex(navGraphVertexRawPtr.GetNavGraphVertexIdx());
		if (vertexNodeIndex != currentNodeIndex)
		{
			KY_FORWARD_ERROR_NO_LOG(OpenOrUpdateGraphVertex(activeData, navGraphVertexRawPtr, navTag, vertexNodeIndex, currentNodeIndex));
		}
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::OpenOrUpdateHalfEdge(ActiveData* activeData, const NavHalfEdgeRawPtr& halfEdgeRawPtr,
	AStarNodeIndex halfEdgeNodeIndex, const Vec3f& startPosOfEdge, const Vec3f& endPosOfEdge, const NavTag& navTag, AStarNodeIndex currentNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;

	if (halfEdgeNodeIndex == AStarNodeIndex_Invalid)
	{
		NavHalfEdgeRawPtr pairHalfEdgeOfNewNode;
		if (ShouldOpenHalfEdgeNode(activeData, halfEdgeRawPtr, pairHalfEdgeOfNewNode))
		{
			// no index for this Edge -> it has never been encoutered, it is a new node
			KY_FORWARD_ERROR_NO_LOG(CreateNewHalfEdgeNode(activeData, halfEdgeRawPtr, pairHalfEdgeOfNewNode, startPosOfEdge, endPosOfEdge, currentNodeIndex));
		}
	}
	else
	{
		if (halfEdgeNodeIndex != aStarNodes[currentNodeIndex].GetIndexOfPredecessor())
		{
			// A node has already be created for this halfEdge -> we update it
			KY_FORWARD_ERROR_NO_LOG(UpdateOpenedOrClosedNode(navTag, halfEdgeNodeIndex, currentNodeIndex));
		}
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
bool AStarTraversal<AStarCustomizer>::ShouldOpenHalfEdgeNode(ActiveData* /*activeData*/, const NavHalfEdgeRawPtr& halfEdge, NavHalfEdgeRawPtr& pairHalfEdgeOfNewNode)
{
	// check if the edge can be crossed
	if (halfEdge.IsHalfEdgeCrossable<AStarCustomizer>(pairHalfEdgeOfNewNode, &m_aStarCustomizer) == false)
		return false; // edge no crossable, do not create a node

	const NavFloor* navFloor = pairHalfEdgeOfNewNode.GetNavFloor();
	const NavFloorBlob* navFloorBlob = navFloor->GetNavFloorBlob();

	const NavTriangleIdx triangleIdx = NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(pairHalfEdgeOfNewNode.GetHalfEdgeIdx());
	if (CanTraverse(navFloorBlob->GetNavTag(triangleIdx)) == false)
		return false;

	// Check whether crossing the edge leads to a dead end
	if (navFloor->GetNavFloorToNavGraphLinks()->IsTriangleLinkedToGraph(triangleIdx) == false)
	{
		const NavHalfEdgeIdx nextIdx =  NavFloorBlob::NavHalfEdgeIdxToNextNavHalfEdgeIdx(pairHalfEdgeOfNewNode.GetHalfEdgeIdx());
		const NavHalfEdgeIdx prevtIdx = NavFloorBlob::NavHalfEdgeIdxToPrevNavHalfEdgeIdx(pairHalfEdgeOfNewNode.GetHalfEdgeIdx());

		const NavHalfEdge* const halfEdges = navFloorBlob->m_navHalfEdges.GetValues();

		const NavTriangleRawPtr triangleRawPtr(pairHalfEdgeOfNewNode.m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(pairHalfEdgeOfNewNode.GetHalfEdgeIdx()));

		if (halfEdges[nextIdx].GetHalfEdgeType() == EDGETYPE_OBSTACLE && halfEdges[prevtIdx].GetHalfEdgeType() == EDGETYPE_OBSTACLE &&
			triangleRawPtr != m_destTriangleRawPtr)
			return false; // edge crossable but leading to a dead end, do not create a node
	}

	return true;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::OpenOrUpdateGraphVertex(ActiveData* activeData, const NavGraphVertexRawPtr& navGraphVertexRawPtr,
	const NavTag& navTag, AStarNodeIndex vertexNodeIndex, AStarNodeIndex currentNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;

	if (vertexNodeIndex == AStarNodeIndex_Invalid)
	{
		if (ShouldOpenGraphVertexNode(activeData, navGraphVertexRawPtr))
		{
			// no index for this vertex -> it has never been encoutered, it is a new node
			KY_FORWARD_ERROR_NO_LOG(CreateNewGraphVertexNode(navGraphVertexRawPtr, navTag, currentNodeIndex));
		}
	}
	else
	{
		if (vertexNodeIndex != aStarNodes[currentNodeIndex].GetIndexOfPredecessor())
			// A node has already be created for this vertex -> we update it
			KY_FORWARD_ERROR_NO_LOG(UpdateOpenedOrClosedNode(navTag, vertexNodeIndex, currentNodeIndex));
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
bool AStarTraversal<AStarCustomizer>::ShouldOpenGraphVertexNode(ActiveData* /*activeData*/, const NavGraphVertexRawPtr& /*navGraphVertexRawPtr*/)
{
	return true;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::UpdateDestNode(const NavTag& navTag, AStarNodeIndex currentNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;
	AstarTraversalBinHeap& binaryHeap = m_astarContext->m_traversalBinHeap;

	// check if we do not have to update the DestNode
	AStarNode& destNode = aStarNodes[AStarNodeIndex_DestNode];
	if (destNode.m_indexInBinaryHeap == IndexInBinHeap_UnSet)
	{
		// check for memory to insert a newElement in the binary heap
		KY_FORWARD_ERROR_NO_LOG(m_astarContext->CheckTraversalBinaryHeapMemory());

		// dest node has not been open yet. We add it in the Byndary Heap
		AStarNode& predecessor = aStarNodes[currentNodeIndex];
		destNode.m_costFromStart = predecessor.m_costFromStart + EvaluateEdgeCostOnNavMesh(predecessor, destNode); // so far the dest is bound to be in the navMesh
		destNode.SetIndexOfPredecessor(currentNodeIndex);

		binaryHeap.Insert(AStarNodeIndex_DestNode);
	}
	else
	{
		// destNode is already opened ->  we update it
		KY_FORWARD_ERROR_NO_LOG(UpdateOpenedOrClosedNode(navTag, AStarNodeIndex_DestNode, currentNodeIndex));
	}

	return KY_SUCCESS;
}


template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::ExploreHalfEdgesOfTriangle(ActiveData* activeData, const NavTriangleRawPtr& triangleRawPtr,
	AStarNodeIndex currentNodeIndex)
{
	AstarNodeIndexInGrid& edgeIndexGrid = m_astarContext->m_edgeIndexGrid;

	AstarNodeIndexInGrid::NavFloorToNodeIndices* navFloorToNodeIndicesPtr = KY_NULL;

	// retrieve a pointer the NodeIndices of this navFloorBlob HalfEdges
	if (KY_FAILED(edgeIndexGrid.GetNavFloorToNodeIndices(activeData, triangleRawPtr.m_navFloorRawPtr, navFloorToNodeIndicesPtr)))
	{
		KY_LOG_WARNING( ("This traversal reached the maximum size of propagation memory in the activeData"));
		return KY_ERROR;
	}

	KY_ASSERT(navFloorToNodeIndicesPtr != KY_NULL);

	NavHalfEdgeRawPtr edgeRawPtr[3];
	AStarNodeIndex edgeIndex[3];
	Vec3f v0f, v1f, v2f;
	Vec3f* vertices[4]; // the vertices of the Triangle
	vertices[0] = &v0f;
	vertices[1] = &v1f;
	vertices[2] = &v2f;
	vertices[3] = &v0f;

	const NavHalfEdgeIdx firstHalfEdgeIdx = NavFloorBlob::NavTriangleIdxToFirstNavHalfEdgeIdx(triangleRawPtr.GetTriangleIdx());
	const NavTag& navTag = triangleRawPtr.GetNavTag();

	// compute NavHalfEdgeRawPtr and retrieve pointer to Nodeindex
	for (KyUInt32 i = 0; i < 3; ++i)
	{
		edgeRawPtr[i].Set(triangleRawPtr.m_navFloorRawPtr, firstHalfEdgeIdx + i);
		edgeIndex[i] = navFloorToNodeIndicesPtr->GetAStarNodeIndex(firstHalfEdgeIdx + i);
	}

	// be sure it won't be used !
	navFloorToNodeIndicesPtr = KY_NULL;

	if (edgeIndex[0] == AStarNodeIndex_Invalid || edgeIndex[1] == AStarNodeIndex_Invalid || edgeIndex[2] == AStarNodeIndex_Invalid)
	{
		// retrieve the three vertices of the triangles
		triangleRawPtr.GetVerticesPos3f(v0f, v1f, v2f);
	}

	for (KyUInt32 i = 0; i < 3; ++i)
	{
		KY_FORWARD_ERROR_NO_LOG(OpenOrUpdateHalfEdge(activeData, edgeRawPtr[i], edgeIndex[i], *vertices[i], *vertices[i + 1], navTag, currentNodeIndex));
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
KyResult AStarTraversal<AStarCustomizer>::UpdateOpenedOrClosedNode(const NavTag& navTag, AStarNodeIndex neighbourNodeIndex, AStarNodeIndex currentNodeIndex)
{
	WorkingMemArray<AStarNode>& aStarNodes = m_astarContext->m_aStarNodes;

	AStarNode& newPredecessor = aStarNodes[currentNodeIndex];
	AStarNode& neighbourNode = aStarNodes[neighbourNodeIndex];

	const KyFloat32 newCost = newPredecessor.m_costFromStart + EvaluateEdgeCost(newPredecessor , neighbourNode, navTag);

	if (newCost >= neighbourNode.m_costFromStart)
		return KY_SUCCESS;

	neighbourNode.SetIndexOfPredecessor(currentNodeIndex);
	neighbourNode.m_costFromStart = newCost;
	return m_astarContext->InsertOrUpdateInBinHeapTraversal(neighbourNode, neighbourNodeIndex);
}

template<class AStarCustomizer>
KyFloat32 AStarTraversal<AStarCustomizer>::EvaluateEdgeCost(const AStarNode& node1, const AStarNode& node2, const NavTag& navTag)
{
	if (node1.GetNodeType() != NodeType_NavGraphVertex || node2.GetNodeType() != NodeType_NavGraphVertex)
		return EvaluateEdgeCostOnNavMesh(node1, node2);

	const KyUInt32 indexOfvertexRawPtr1 = node1.GetIdxOfRawPtrData();
	const KyUInt32 indexOfvertexRawPtr2 = node2.GetIdxOfRawPtrData();
	NavGraphVertexRawPtr& navGraphVertexRawPtr1 = m_astarContext->m_vertexRawPtrNodes[indexOfvertexRawPtr1];
	NavGraphVertexRawPtr& navGraphVertexRawPtr2 = m_astarContext->m_vertexRawPtrNodes[indexOfvertexRawPtr2];

	if (navGraphVertexRawPtr1.GetNavGraph() != navGraphVertexRawPtr2.GetNavGraph())
		return EvaluateEdgeCostOnNavMesh(node1, node2);

	return EvaluateGraphEdgeCost(node1, node2, navTag);
}

template<class AStarCustomizer>
void AStarTraversal<AStarCustomizer>::DisplayListPropagation(Database* database)
{
	ScopedDisplayList* displayList = KY_NEW ScopedDisplayList(database->GetWorld(), "Astar Propagation", "AStar", DisplayList_Enable);
	
	const Vec3f altOffset(0.f, 0.f, 0.01f);

	for (AStarNodeIndex index = 0; index < m_astarContext->m_aStarNodes.GetCount(); ++index)
	{
		const AStarNode& aStarNode = m_astarContext->m_aStarNodes[index];
		if (aStarNode. GetIndexOfPredecessor() != AStarNodeIndex_Invalid)
		{
			const Vec3f& nodePosition = aStarNode.m_nodePosition;
			const AStarNode& predecessorNode  = m_astarContext->m_aStarNodes[aStarNode.GetIndexOfPredecessor()];
			displayList->PushLine(predecessorNode.m_nodePosition + altOffset, nodePosition + altOffset,VisualColor::Orange);
		}
	}

	displayList->Commit();
	delete displayList;
}
