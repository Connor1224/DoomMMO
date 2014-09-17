/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY

// TODO: put all non template functions in a base class 
// to avoid template<class AStarCustomizer> everywhere


namespace Kaim
{

template<class AStarCustomizer>
AStarQuery<AStarCustomizer>::AStarQuery() : m_astarTraversal()
{
	m_astarTraversal.m_baseAStarQuery = (BaseAStarQuery*)this;
}

template<class AStarCustomizer>
AStarQuery<AStarCustomizer>::AStarQuery(const AStarCustomizer& customzier) : m_astarTraversal(customzier)
{
	m_astarTraversal.m_baseAStarQuery = (BaseAStarQuery*)this;
}
template<class AStarCustomizer>
AStarQuery<AStarCustomizer>::~AStarQuery() {}

template<class AStarCustomizer>
void AStarQuery<AStarCustomizer>::Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos)
{
	BaseAStarQuery::Initialize(database, startPos, destPos);
}

template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr)
{
	BaseAStarQuery::SetStartTrianglePtr(startTrianglePtr);
}
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetDestTrianglePtr(const NavTrianglePtr& destTrianglePtr)
{
	BaseAStarQuery::SetDestTrianglePtr(destTrianglePtr);
}
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseAStarQuery::SetPositionSpatializationRange(positionSpatializationRange);
}
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetHookingMode(AStarQueryHookingMode aStarQueryHookingMode)
{
	BaseAStarQuery::SetHookingMode(aStarQueryHookingMode);
}
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetHorizontalHookingMaxDist(KyFloat32 horizontalHookingMaxDist)
{
	BaseAStarQuery::SetHorizontalHookingMaxDist(horizontalHookingMaxDist);
}
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetPathMaxCost(KyFloat32 pathMaxCost)
{
	BaseAStarQuery::SetPathMaxCost(pathMaxCost);
}

template<class AStarCustomizer>
KY_INLINE AStarQueryResult      AStarQuery<AStarCustomizer>::GetResult()                   const { return BaseAStarQuery::GetResult();                   }
template<class AStarCustomizer>
KY_INLINE const Vec3f&          AStarQuery<AStarCustomizer>::GetStartPos()                 const { return BaseAStarQuery::GetStartPos();                 }
template<class AStarCustomizer>
KY_INLINE const Vec3f&          AStarQuery<AStarCustomizer>::GetDestPos()                  const { return BaseAStarQuery::GetDestPos();                  }
template<class AStarCustomizer>
KY_INLINE const NavTrianglePtr& AStarQuery<AStarCustomizer>::GetStartTrianglePtr()         const { return BaseAStarQuery::GetStartTrianglePtr();         }
template<class AStarCustomizer>
KY_INLINE const NavTrianglePtr& AStarQuery<AStarCustomizer>::GetDestTrianglePtr()          const { return BaseAStarQuery::GetDestTrianglePtr();          }
template<class AStarCustomizer>
KY_INLINE Path*                 AStarQuery<AStarCustomizer>::GetPath()                     const { return BaseAStarQuery::GetPath();                     }
template<class AStarCustomizer>
KY_INLINE KyFloat32             AStarQuery<AStarCustomizer>::GetHorizontalHookingMaxDist() const { return BaseAStarQuery::GetHorizontalHookingMaxDist(); }
template<class AStarCustomizer>
KY_INLINE KyFloat32             AStarQuery<AStarCustomizer>::GetPathMaxCost()              const { return BaseAStarQuery::GetPathMaxCost();              }
template<class AStarCustomizer>
KY_INLINE AStarQueryHookingMode AStarQuery<AStarCustomizer>::GetHookingMode()              const { return BaseAStarQuery::GetHookingMode();              }

template<class AStarCustomizer>
KY_INLINE KyUInt32              AStarQuery<AStarCustomizer>::GetNumberOfProcessedNodePerFrame() const { return m_numberOfVisitedNodePerFrame; }
template<class AStarCustomizer>
KY_INLINE const PositionSpatializationRange& AStarQuery<AStarCustomizer>::GetPositionSpatializationRange() const { return BaseAStarQuery::GetPositionSpatializationRange(); }
template<class AStarCustomizer>
KY_INLINE void AStarQuery<AStarCustomizer>::SetNumberOfProcessedNodePerFrame(KyUInt32 numberOfProcessedNodePerFrame) { m_numberOfVisitedNodePerFrame = numberOfProcessedNodePerFrame; }


template<class AStarCustomizer>
inline void AStarQuery<AStarCustomizer>::ReleaseWorkingMemoryOnCancelDuringProcess(WorkingMemory* workingMemory)
{
	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	SetResult(ASTAR_DONE_COMPUTATION_CANCELED);
	SetFinish(workingMemory);
}

template<class AStarCustomizer>
inline void AStarQuery<AStarCustomizer>::Advance(WorkingMemory* workingMemory)
{
	++m_advanceCount;
	m_processStatus = QueryInProcess;

	QueryUtils queryUtils(m_database, workingMemory);

	switch (GetResult())
	{
	case ASTAR_NOT_PROCESSED :
		{
			if (m_database->IsClear())
			{
				SetResult(ASTAR_DONE_START_OUTSIDE);
				SetFinish(workingMemory);
				return;
			}

			m_astarTraversal.m_astarContext = workingMemory->GetAStarTraversalContext();
			m_lastAdvanceFrameIdx = m_database->m_frameIdx;

			if (KY_FAILED(ComputeStartTriangle(queryUtils)))
			{
				SetFinish(workingMemory);
				return;
			}

			if (KY_FAILED(ComputeDestTriangle(queryUtils)))
			{
				SetFinish(workingMemory);
				return;
			}

			if (KY_FAILED(m_astarTraversal.InitializeContextAndStartAndDestNode(queryUtils, m_startInsidePos3f, m_destInsidePos3f, m_startTrianglePtr, m_destTrianglePtr)))
			{
				SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);
				SetFinish(workingMemory);
				return;
			}

			if (m_astarTraversal.IsThereNodeToVisit() == false)
			{
				if (m_astarTraversal.HasExplorationReachedDestNode() == false)
				{
					// no neighbour for the start nodes
					SetResult(ASTAR_DONE_PATH_NOT_FOUND);
					SetFinish(workingMemory);
					return;
				}

				SetResult(ASTAR_PROCESSING_TRAVERSAL_DONE);
			}
			else
				SetResult(ASTAR_PROCESSING_TRAVERSAL);

			break;
		}
	case ASTAR_PROCESSING_TRAVERSAL :
		{
			if(m_lastAdvanceFrameIdx != m_database->m_frameIdx)
			{
				if(m_astarTraversal.m_astarContext->m_edgeIndexGrid.HasNavDataChanged(m_database))
				{
					SetResult(ASTAR_DONE_NAVDATA_CHANGED);
					SetFinish(workingMemory);
					return;
				}

				m_lastAdvanceFrameIdx = m_database->m_frameIdx;
			}

			for(KyUInt32 numberOfNode = 0; numberOfNode < m_numberOfVisitedNodePerFrame; ++numberOfNode)
			{
				bool doesPathMaxDistReached = false;
				if (KY_FAILED(m_astarTraversal.VisitNode(queryUtils, doesPathMaxDistReached)))
				{
					if (doesPathMaxDistReached)
						SetResult(ASTAR_DONE_PATH_MAX_COST_REACHED);
					else
						SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);

					SetFinish(workingMemory);
					return;
				}

				if (m_astarTraversal.IsThereNodeToVisit() == false)
					break;
			}

			if (m_astarTraversal.IsThereNodeToVisit() == false)
			{
				// Visual debugging
				// Uncomment this line to send the result of the propagation via the visual debugging
				// m_astarTraversal.DisplayListPropagation(m_database);

				if (m_astarTraversal.HasExplorationReachedDestNode() == false)
				{
					SetResult(ASTAR_DONE_PATH_NOT_FOUND);
					SetFinish(workingMemory);
					return;
				}

				SetResult(ASTAR_PROCESSING_TRAVERSAL_DONE);
			}

			break;
		}

	case ASTAR_PROCESSING_TRAVERSAL_DONE :
		{
			if(m_lastAdvanceFrameIdx != m_database->m_frameIdx)
			{
				if(m_astarTraversal.m_astarContext->m_edgeIndexGrid.HasNavDataChanged(m_database))
				{
					SetResult(ASTAR_DONE_NAVDATA_CHANGED);
					SetFinish(workingMemory);
					return;
				}

				m_lastAdvanceFrameIdx = m_database->m_frameIdx;
			}

			PathRefinerContext* refinerContext = workingMemory->GetPathRefinerContext();
			if (KY_FAILED(refinerContext->InitFromAstarTraversalContex(workingMemory, workingMemory->m_astarContext)))
			{
				SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);
				SetFinish(workingMemory);
				return;
			}

			SetResult(ASTAR_PROCESSING_REFINING_INIT);
			break;
		}

	case ASTAR_PROCESSING_REFINING_INIT :
		{
			if (KY_FAILED(CheckNavDataChange(workingMemory)))
				return;

			if (KY_FAILED(BuildBinaryHeap(queryUtils)))
			{
				SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);
				SetFinish(workingMemory);
				return;
			}

			PathRefinerContext* refinerContext = workingMemory->GetPathRefinerContext();
			if (refinerContext->m_currentNodeIdx == refinerContext->m_refinerNodes.GetCount())
			{
				if (refinerContext->m_refinerBinHeap.IsEmpty())
					SetResult(ASTAR_PROCESSING_PATHCLAMPING_INIT);
				else
					SetResult(ASTAR_PROCESSING_REFINING);
			}

			break;
		}
	case ASTAR_PROCESSING_REFINING :
		{
			if (KY_FAILED(CheckNavDataChange(workingMemory)))
				return;

			PathRefinerContext* refinerContext = workingMemory->GetPathRefinerContext();
			for(KyUInt32 cangoTestCount = 0; cangoTestCount < m_numberOfCanGoTestInRefinerPerFrame;)
			{
				if (KY_FAILED(RefineOneNode(refinerContext, queryUtils, cangoTestCount)))
				{
					SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);
					SetFinish(workingMemory);
					return;
				}

				if (refinerContext->m_refinerBinHeap.IsEmpty())
					break;
			}

			if (refinerContext->m_refinerBinHeap.IsEmpty())
			{
				// Visual debugging
				// Uncomment this line to send the result of the path refiner, befoire the path is clamped
				// DisplayListRefining(refinerContext);

				SetResult(ASTAR_PROCESSING_PATHCLAMPING_INIT);
			}

			break;
		}
	case ASTAR_PROCESSING_PATHCLAMPING_INIT :
		{
			if (KY_FAILED(CheckNavDataChange(workingMemory)))
				return;

			if (KY_FAILED(workingMemory->m_clamperContext->InitFromRefinerContext(workingMemory, workingMemory->GetPathRefinerContext())))
			{
				SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);
				SetFinish(workingMemory);
				return;
			}

			SetResult(ASTAR_PROCESSING_PATHCLAMPING);
			break;
		}
	case ASTAR_PROCESSING_PATHCLAMPING :
		{
			if (KY_FAILED(CheckNavDataChange(workingMemory)))
				return;

			PathClamperContext* pathClamperContext = workingMemory->GetPathClamperContext();
			for(KyUInt32 intersectionTestCount = 0; intersectionTestCount < m_numberOfIntersectionTestPerFrame;)
			{
				ClampResult clampResult = (ClampResult)ClampOneEdge(pathClamperContext, queryUtils, intersectionTestCount);
				if (clampResult != ClampResult_SUCCESS)
				{
					if (clampResult == ClampResult_FAIL_CANGOHIT)
						SetResult(ASTAR_DONE_COMPUTATION_ERROR);
					else
						SetResult(ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY);

					SetFinish(workingMemory);
					return;
				}

				if (pathClamperContext->IsClampingDone())
					break;
			}

			if (pathClamperContext->IsClampingDone())
			{
				SetResult(ASTAR_PROCESSING_PATHBUILDING);
			}

			break;
		}
	case ASTAR_PROCESSING_PATHBUILDING :
		{
			if (KY_FAILED(CheckNavDataChange(workingMemory)))
				return;

			ComputePathFromPathClamperContext(queryUtils, m_startPos3f, m_destPos3f);
			SetResult(ASTAR_DONE_PATH_FOUND);
			SetFinish(workingMemory);
			break;
		}
	default:
		{
			m_lastAdvanceFrameIdx = m_database->m_frameIdx;
			SetFinish(workingMemory);
			break;
		}
	}
}

template<class AStarCustomizer>
KY_INLINE bool AStarQuery<AStarCustomizer>::HasNavDataChanged()
{
	return m_astarTraversal.m_astarContext->m_edgeIndexGrid.HasNavDataChanged(m_database);
}

template<class AStarCustomizer>
inline void AStarQuery<AStarCustomizer>::SetFinish(WorkingMemory* workingMemory)
{
	m_processStatus = QueryDone; // TODO - CHANGE THIS!
	m_dataBaseChangeIdx = m_database ? m_database->m_navdataChangeIdx : 0;

	workingMemory->m_astarContext->ReleaseWorkingMemory();
	workingMemory->m_refinerContext->ReleaseWorkingMemory();
	workingMemory->m_clamperContext->ReleaseWorkingMemory();
	workingMemory->m_navDataChangeIndexInGrid->ReleaseWorkingMemoryBuffer();
}

template<class AStarCustomizer>
inline KyResult AStarQuery<AStarCustomizer>::CheckNavDataChange(WorkingMemory* workingMemory)
{
	if(m_lastAdvanceFrameIdx != m_database->m_frameIdx)
	{
		if(workingMemory->m_navDataChangeIndexInGrid->HasNavDataChanged(m_database))
		{
			SetResult(ASTAR_DONE_NAVDATA_CHANGED);
			SetFinish(workingMemory);
			return KY_ERROR;
		}

		m_lastAdvanceFrameIdx = m_database->m_frameIdx;
	}

	return KY_SUCCESS;
}

template<class AStarCustomizer>
inline KyResult AStarQuery<AStarCustomizer>::ComputeStartTriangle(QueryUtils& queryUtils)
{
	KY_DEBUG_ASSERTN(queryUtils.m_database->GetActiveData() != KY_NULL, ("the QueryUtils need a valid activeData"));

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_positionSpatializationRange) == true)
	{
		if(m_astarTraversal.m_aStarCustomizer.CanTraverse(m_startTrianglePtr.GetNavTag()) == true)
		{
			m_startInsidePos3f = m_startPos3f;
			return KY_SUCCESS;
		}
	}

	// the position is outside the NavMesh or in an invalid navtag. Try to hook on the NavMesh
	if (TryToHookOnNavMesh(queryUtils, m_startPos3f, m_startInsidePos3f, m_startTrianglePtr) == KY_SUCCESS)
	{
		return KY_SUCCESS;
	}

	if (m_startTrianglePtr.IsValid() == false)
		SetResult(ASTAR_DONE_START_OUTSIDE);
	else
		SetResult(ASTAR_DONE_START_NAVTAG_FORBIDDEN);

	return KY_ERROR;
}


template<class AStarCustomizer>
inline KyResult AStarQuery<AStarCustomizer>::ComputeDestTriangle(QueryUtils& queryUtils)
{
	KY_DEBUG_ASSERTN(queryUtils.m_database->GetActiveData() != KY_NULL, ("the QueryUtils need a valid activeData"));

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_destTrianglePtr, m_destPos3f, m_positionSpatializationRange) == true)
	{
		if(m_astarTraversal.m_aStarCustomizer.CanTraverse(m_destTrianglePtr.GetNavTag()) == true)
		{
			m_destInsidePos3f = m_destPos3f;
			return KY_SUCCESS;
		}
	}

	// the position is outside the NavMesh or in an invalid navtag. Try to hook on the NavMesh
	if (TryToHookOnNavMesh(queryUtils, m_destPos3f, m_destInsidePos3f, m_destTrianglePtr) == KY_SUCCESS)
	{
		return KY_SUCCESS;
	}

	if (m_destTrianglePtr.IsValid() == false)
		SetResult(ASTAR_DONE_END_OUTSIDE);
	else
		SetResult(ASTAR_DONE_END_NAVTAG_FORBIDDEN);

	return KY_ERROR;
}

template<class AStarCustomizer>
inline KyResult AStarQuery<AStarCustomizer>::TryToHookOnNavMesh(QueryUtils& queryUtils, const Vec3f& inputOutsidePos, Vec3f& outputInsidePos, NavTrianglePtr& outputNavTrianglePtr)
{
	if (GetHookingMode() != ASTAR_TRY_TO_HOOK_ON_NAVMESH)
		return KY_ERROR;

	InsidePosFromOutsidePosQuery<AStarCustomizer> insidePosFromOutsidePosQuery(m_astarTraversal.m_aStarCustomizer);
	insidePosFromOutsidePosQuery.Initialize(m_database, inputOutsidePos);
	insidePosFromOutsidePosQuery.SetPositionSpatializationRange(m_positionSpatializationRange);
	insidePosFromOutsidePosQuery.SetHorizontalTolerance(GetHorizontalHookingMaxDist()); // look up to +/- GetHorizontalHookingMaxDist() on x,y.
	insidePosFromOutsidePosQuery.SetDistFromObstacle(0.1f); // 10 cm from the border
	insidePosFromOutsidePosQuery.PerformQuery(queryUtils.m_workingMemory);

	if (insidePosFromOutsidePosQuery.GetResult() == INSIDEPOSFROMOUTSIDE_DONE_POS_FOUND)
	{
		outputInsidePos = insidePosFromOutsidePosQuery.GetInsidePos();
		outputNavTrianglePtr = insidePosFromOutsidePosQuery.GetInsidePosTrianglePtr();
		return KY_SUCCESS;
	}

	return KY_ERROR;
}

template<class AStarCustomizer>
inline KyResult AStarQuery<AStarCustomizer>::RefineOneNode(PathRefinerContext* refinerContext, QueryUtils& queryUtils, KyUInt32& cangoTestDone)
{
	RayCanGoQuery<AStarCustomizer> rayCanGoQuery(m_astarTraversal.m_aStarCustomizer);
	RayCastQuery<AStarCustomizer> rayCastToPointOfEdge1(m_astarTraversal.m_aStarCustomizer);
	RayCastQuery<AStarCustomizer> rayCastToPointOfEdge2(m_astarTraversal.m_aStarCustomizer);

	RefinerBinHeap& refinerBinHeap = refinerContext->m_refinerBinHeap;
	WorkingMemArray<RefinerNode>& refinerNodes = refinerContext->m_refinerNodes;
	WorkingMemArray<NavGraphVertexRawPtr>& vertexRawPtrNodes = refinerContext->m_vertexRawPtrNodes;
	WorkingMemArray<NavTriangleRawPtr>& triangleRawPtrNodes = refinerContext->m_triangleRawPtrNodes;

	RefinerNodeIndex currentNodeIndex;

	refinerBinHeap.ExtractFirst(currentNodeIndex);

	RefinerNode* currentNode = &refinerNodes[currentNodeIndex];
	RefinerNodeIndex prevNodeIdx = currentNode->m_predecessorNodeIdx;
	RefinerNodeIndex nextNodeIdx = currentNode->m_nextNodeIdx;

	RefinerNode* prevNode = &refinerNodes[prevNodeIdx];
	RefinerNode* nextNode = &refinerNodes[nextNodeIdx];

	const Vec3f& prevNodePos = prevNode->m_nodePosition;
	const Vec3f& nextNodePos = nextNode->m_nodePosition;

	NavTrianglePtr prevTrianglePtr;

	if(prevNode->GetNodeType() != NodeType_NavGraphVertex)
	{
		prevTrianglePtr = NavTrianglePtr(triangleRawPtrNodes[prevNode->GetIdxOfRawPtrData()]);
	}
	else
	{
		const NavGraphVertexRawPtr& nodeGraphVertexRawPtr = vertexRawPtrNodes[prevNode->GetIdxOfRawPtrData()];
		NavGraphLink* navGraphLink = nodeGraphVertexRawPtr.GetGraphVertexData().m_navGraphLink;
		KY_ASSERT(navGraphLink != KY_NULL && navGraphLink->IsValid());
		prevTrianglePtr = NavTrianglePtr(navGraphLink->m_navTriangleRawPtr);
	}

	rayCanGoQuery.Initialize(queryUtils.m_database, prevNodePos, nextNodePos);
	rayCanGoQuery.SetStartIntegerPos(prevNode->m_nodeIntegerPos);
	rayCanGoQuery.SetDestIntegerPos(nextNode->m_nodeIntegerPos);
	rayCanGoQuery.SetStartTrianglePtr(prevTrianglePtr);
	rayCanGoQuery.SetPositionSpatializationRange(GetPositionSpatializationRange());

	rayCanGoQuery.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
	++cangoTestDone;
	if (rayCanGoQuery.GetResult() == RAYCANGO_DONE_SUCCESS)
	{
		prevNode->m_nextNodeIdx = nextNodeIdx;
		nextNode->m_predecessorNodeIdx = prevNodeIdx;
		KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, prevNodeIdx, prevNode));
		KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, nextNodeIdx, nextNode));

		return KY_SUCCESS;
	}

	if (currentNode->m_refinerCost < 0.001f)
		// angle is flat, avoid floating point issue which can cause infinite loop (limit is ~177 degrees = 3.1 rad)
		return KY_SUCCESS;

	const Vec3f& currentNodePos = currentNode->m_nodePosition;

	Vec2f nodeToPrev(prevNodePos.x - currentNodePos.x, prevNodePos.y - currentNodePos.y);
	Vec2f nodeToNext(nextNodePos.x - currentNodePos.x, nextNodePos.y - currentNodePos.y);

	const KyFloat32 nodeToPrevLength = nodeToPrev.Normalize(); // edge1
	const KyFloat32 nodeToNextLength = nodeToNext.Normalize(); // edge2

	KY_ASSERT(currentNode->GetNodeType() == NodeType_NavMeshEdge);
	NavTrianglePtr nodeTrianglePtr(triangleRawPtrNodes[currentNode->GetIdxOfRawPtrData()]);

	const KyFloat32 m_minEdgeLength = 3.f;

	bool canSplitEdgeFromPrevInTheMiddle = nodeToPrevLength > 2 * m_minEdgeLength;
	bool canSplitEdgeToNextInTheMiddle = nodeToNextLength > 2 * m_minEdgeLength;

	bool tryANewPointOnEdge1 = canSplitEdgeFromPrevInTheMiddle || nodeToPrevLength > m_minEdgeLength * 1.3f;
	bool tryANewPointOnEdge2 = canSplitEdgeToNextInTheMiddle || nodeToNextLength > m_minEdgeLength * 1.3f;

	KyFloat32 distFromNodeToPointOnEdge1 = canSplitEdgeFromPrevInTheMiddle ? nodeToPrevLength * 0.5f : nodeToPrevLength - m_minEdgeLength;
	KyFloat32 distFromNodeToPointOnEdge2 = canSplitEdgeToNextInTheMiddle ? nodeToNextLength * 0.5f : nodeToNextLength - m_minEdgeLength;

	if (tryANewPointOnEdge1 == false && tryANewPointOnEdge2 == false)
		return KY_SUCCESS;

	if (tryANewPointOnEdge1)
	{
		// retrieve triangle and IntegerPos along edge1
		rayCastToPointOfEdge1.Initialize(queryUtils.m_database, currentNodePos, nodeToPrev, distFromNodeToPointOnEdge1);
		rayCastToPointOfEdge1.SetStartIntegerPos(currentNode->m_nodeIntegerPos);
		rayCastToPointOfEdge1.SetStartTrianglePtr(nodeTrianglePtr);
		rayCastToPointOfEdge1.SetPositionSpatializationRange(GetPositionSpatializationRange());
		rayCastToPointOfEdge1.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
		++cangoTestDone;
		if (rayCastToPointOfEdge1.GetResult() != RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED)
			return KY_SUCCESS;

		RayCanGoQuery<AStarCustomizer>& checkCanGo = rayCanGoQuery;
		checkCanGo.Initialize(queryUtils.m_database, rayCastToPointOfEdge1.GetArrivalPos(), prevNodePos);
		checkCanGo.SetStartIntegerPos(rayCastToPointOfEdge1.GetArrivalIntegerPos());
		checkCanGo.SetStartTrianglePtr(rayCastToPointOfEdge1.GetArrivalTrianglePtr());
		checkCanGo.SetDestIntegerPos(prevNode->m_nodeIntegerPos);
		checkCanGo.SetPositionSpatializationRange(GetPositionSpatializationRange());
		checkCanGo.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
		++cangoTestDone;
		if (checkCanGo.GetResult() != RAYCANGO_DONE_SUCCESS)
			return KY_SUCCESS;
	}

	if (tryANewPointOnEdge2)
	{
		// retrieve triangle and IntegerPos along edge2
		rayCastToPointOfEdge2.Initialize(queryUtils.m_database, currentNodePos, nodeToNext, distFromNodeToPointOnEdge2);
		rayCastToPointOfEdge2.SetStartIntegerPos(currentNode->m_nodeIntegerPos);
		rayCastToPointOfEdge2.SetStartTrianglePtr(nodeTrianglePtr);
		rayCastToPointOfEdge2.SetPositionSpatializationRange(GetPositionSpatializationRange());
		rayCastToPointOfEdge2.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
		++cangoTestDone;
		if (rayCastToPointOfEdge2.GetResult() != RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED)
			return KY_SUCCESS;

		RayCanGoQuery<AStarCustomizer>& checkCanGo = rayCanGoQuery;
		checkCanGo.Initialize(queryUtils.m_database, rayCastToPointOfEdge2.GetArrivalPos(), nextNodePos);
		checkCanGo.SetStartIntegerPos(rayCastToPointOfEdge2.GetArrivalIntegerPos());
		checkCanGo.SetStartTrianglePtr(rayCastToPointOfEdge2.GetArrivalTrianglePtr());
		checkCanGo.SetDestIntegerPos(nextNode->m_nodeIntegerPos);
		checkCanGo.SetPositionSpatializationRange(GetPositionSpatializationRange());
		checkCanGo.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
		++cangoTestDone;
		if (checkCanGo.GetResult() != RAYCANGO_DONE_SUCCESS)
			return KY_SUCCESS;
	}


	if (tryANewPointOnEdge1 && tryANewPointOnEdge2)
	{
		RayCanGoQuery<AStarCustomizer>& rayCanGoFromMiddles = rayCanGoQuery;
		rayCanGoFromMiddles.Initialize(queryUtils.m_database, rayCastToPointOfEdge1.GetArrivalPos(), rayCastToPointOfEdge2.GetArrivalPos());
		rayCanGoFromMiddles.SetStartIntegerPos(rayCastToPointOfEdge1.GetArrivalIntegerPos());
		rayCanGoFromMiddles.SetStartTrianglePtr(rayCastToPointOfEdge1.GetArrivalTrianglePtr());
		rayCanGoFromMiddles.SetDestIntegerPos(rayCastToPointOfEdge2.GetArrivalIntegerPos());
		rayCanGoFromMiddles.SetPositionSpatializationRange(GetPositionSpatializationRange());

		rayCanGoFromMiddles.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
		++cangoTestDone;
		if (rayCanGoFromMiddles.GetResult() == RAYCANGO_DONE_SUCCESS)
		{
			const RefinerNodeIndex newNodeIdx = (RefinerNodeIndex)refinerNodes.GetCount();
			if(KY_FAILED(refinerNodes.PushBack(RefinerNode())))
				return KY_ERROR;

			currentNode = &refinerNodes[currentNodeIndex];
			nextNode = &refinerNodes[nextNodeIdx];
			RefinerNode* newNode = &refinerNodes[newNodeIdx];

			triangleRawPtrNodes[currentNode->GetIdxOfRawPtrData()] = rayCastToPointOfEdge1.GetArrivalTrianglePtr().GetRawPtr();

			const KyUInt32 newNodeRawPtrDataIdx = triangleRawPtrNodes.GetCount();
			if(KY_FAILED(triangleRawPtrNodes.PushBack(rayCastToPointOfEdge2.GetArrivalTrianglePtr().GetRawPtr())))
				return KY_ERROR;

			currentNode->m_nodePosition = rayCastToPointOfEdge1.GetArrivalPos();
			currentNode->m_nodeIntegerPos = rayCastToPointOfEdge1.GetArrivalIntegerPos();

			newNode->m_nodePosition = rayCastToPointOfEdge2.GetArrivalPos();
			newNode->m_nodeIntegerPos = rayCastToPointOfEdge2.GetArrivalIntegerPos();

			newNode->SetNodeType(NodeType_NavMeshEdge);
			newNode->SetIdxOfRawPtrData(newNodeRawPtrDataIdx);

			currentNode->m_nextNodeIdx = newNodeIdx;
			newNode->m_predecessorNodeIdx = currentNodeIndex;

			newNode->m_nextNodeIdx = nextNodeIdx;
			nextNode->m_predecessorNodeIdx = newNodeIdx;

			// angle did not change for prevNode and NextNode

			KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, currentNodeIndex, currentNode));
			KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, newNodeIdx,newNode));
			return KY_SUCCESS;
		}
	}
	else
	{
		if (tryANewPointOnEdge1)
		{
			RayCanGoQuery<AStarCustomizer>& rayCanGoFromMiddle1 = rayCanGoQuery;
			rayCanGoFromMiddle1.Initialize(queryUtils.m_database, rayCastToPointOfEdge1.GetArrivalPos(), nextNodePos);
			rayCanGoFromMiddle1.SetStartIntegerPos(rayCastToPointOfEdge1.GetArrivalIntegerPos());
			rayCanGoFromMiddle1.SetStartTrianglePtr(rayCastToPointOfEdge1.GetArrivalTrianglePtr());
			rayCanGoFromMiddle1.SetDestIntegerPos(nextNode->m_nodeIntegerPos);
			rayCanGoFromMiddle1.SetPositionSpatializationRange(GetPositionSpatializationRange());

			rayCanGoFromMiddle1.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
			++cangoTestDone;
			if (rayCanGoFromMiddle1.GetResult() == RAYCANGO_DONE_SUCCESS)
			{
				currentNode->m_nodePosition = rayCastToPointOfEdge1.GetArrivalPos();
				currentNode->m_nodeIntegerPos = rayCastToPointOfEdge1.GetArrivalIntegerPos();
				triangleRawPtrNodes[currentNode->GetIdxOfRawPtrData()] = rayCastToPointOfEdge1.GetArrivalTrianglePtr().GetRawPtr();

				// angle did not change for prevNode
				KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, currentNodeIndex, currentNode));
				KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, nextNodeIdx, nextNode));
				return KY_SUCCESS;
			}
		}
		else
		{
			if (tryANewPointOnEdge2)
			{
				RayCanGoQuery<AStarCustomizer>& rayCanGoFromMiddle2 = rayCanGoQuery;
				rayCanGoFromMiddle2.Initialize(queryUtils.m_database, rayCastToPointOfEdge2.GetArrivalPos(), prevNodePos);
				rayCanGoFromMiddle2.SetStartIntegerPos(rayCastToPointOfEdge2.GetArrivalIntegerPos());
				rayCanGoFromMiddle2.SetStartTrianglePtr(rayCastToPointOfEdge2.GetArrivalTrianglePtr());
				rayCanGoFromMiddle2.SetDestIntegerPos(prevNode->m_nodeIntegerPos);
				rayCanGoFromMiddle2.SetPositionSpatializationRange(GetPositionSpatializationRange());

				rayCanGoFromMiddle2.PerformQueryWithInputCoordPos(queryUtils.m_workingMemory);
				++cangoTestDone;
				if (rayCanGoFromMiddle2.GetResult() == RAYCANGO_DONE_SUCCESS)
				{
					currentNode->m_nodePosition = rayCastToPointOfEdge2.GetArrivalPos();
					currentNode->m_nodeIntegerPos = rayCastToPointOfEdge2.GetArrivalIntegerPos();
					triangleRawPtrNodes[currentNode->GetIdxOfRawPtrData()] = rayCastToPointOfEdge2.GetArrivalTrianglePtr().GetRawPtr();

					// angle did not change for nextNode
					KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, prevNodeIdx, prevNode));
					KY_FORWARD_ERROR_NO_LOG(UpdateNodeInBinaryHeap(refinerContext, currentNodeIndex, currentNode));
					return KY_SUCCESS;
				}
			}
		}
	}

	return KY_SUCCESS;
}


}

