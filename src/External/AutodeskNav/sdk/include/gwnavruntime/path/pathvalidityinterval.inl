/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: JUBA - secondary contact: NOBODY

namespace Kaim
{

KY_INLINE PathValidityInterval::PathValidityInterval() : m_pathValidityStatus(PathValidityStatus_NoPath)
{
	m_queryDynamicOutput = QueryDynamicOutput::Create(QueryDynamicOutput::Config());
}
KY_INLINE PathValidityInterval::~PathValidityInterval() 
{
	Clear(); 
}

KY_INLINE Path*              PathValidityInterval::GetPath()               const { return m_pathEventList.m_path; }
KY_INLINE PathValidityStatus PathValidityInterval::GetPathValidityStatus() const { return m_pathValidityStatus; }

KY_INLINE void PathValidityInterval::SetPathValidityStatus(PathValidityStatus status) { m_pathValidityStatus = status; }
KY_INLINE void PathValidityInterval::SetPath(Path* path) { m_pathEventList.m_path = path; }


template<class NavTagTraversePredicate>
inline KyResult PathValidityInterval::ReStartIntervalComputation(const PositionOnPath& targetOnPath, KyFloat32 minDistanceFromTargetOnPathBackward,
	KyFloat32 minDistanceFromTargetOnPathFrowaard, NavTagTraversePredicate* predicate)
{
	KY_ASSERT(GetPath() != KY_NULL && GetPath()->GetNodeCount() > 1);
	KY_ASSERT(targetOnPath.GetPath() == GetPath());

	ClearBoundsAndEventList();

	// first we check if the targetOnPath is still valid
	m_upperBound = targetOnPath;
	m_lowerBound = targetOnPath;
	m_needToCheckLastEventForRemoval = false;

	const KyUInt32 pathEdgeIdx = targetOnPath.GetPathEdgeIndex();
	PathEdgeType edgeType = GetPath()->GetPathEdgeType(pathEdgeIdx);
	switch (edgeType)
	{
	case PathEdgeType_OnNavMesh :
		{
			// retrieve the triangle of currentTargetOnPath
			// NB up to now, we cannot retrieve a graph edge from this query
			TriangleFromPosQuery triangleFromPosQuery;
			triangleFromPosQuery.Initialize(targetOnPath.GetPath()->m_database, targetOnPath.GetPosition());
			triangleFromPosQuery.PerformQuery();
			if (triangleFromPosQuery.GetResult() != TRIANGLEFROMPOS_DONE_TRIANGLE_FOUND)
			{
				SetPathValidityStatus(Dirty_TargetInvalid);
				ClearBoundsAndEventList();
				return KY_ERROR;
			}

			if (predicate->CanTraverse(triangleFromPosQuery.GetResultTrianglePtr().GetNavTag()) == false)
			{
				SetPathValidityStatus(Dirty_TargetInvalid);
				ClearBoundsAndEventList();
				return KY_ERROR;
			}

			m_lowerTrianglePtr = triangleFromPosQuery.GetResultTrianglePtr();
			m_upperTrianglePtr = triangleFromPosQuery.GetResultTrianglePtr();
			break;
		}

	case PathEdgeType_OnNavGraph :
		{
			NavGraphEdgePtr& navGraphEdgePtr = GetPath()->GetEdgeNavGraphEdgePtr(pathEdgeIdx);
			if (navGraphEdgePtr.IsValid() == false)
			{
				SetPathValidityStatus(Dirty_TargetInvalid);
				ClearBoundsAndEventList();
				return KY_ERROR;
			}

			if (predicate->CanTraverse(navGraphEdgePtr.GetNavTag()) == false)
			{
				SetPathValidityStatus(Dirty_TargetInvalid);
				ClearBoundsAndEventList();
				return KY_ERROR;
			}

			m_lowerTrianglePtr.Invalidate();
			m_upperTrianglePtr.Invalidate();

			break;
		}
	case PathEdgeType_FromOutside:
	case PathEdgeType_ToOutside:
		{
			// Nothing to do when we are on such an edge.
			break;
		}
	default :
		{
			KY_LOG_ERROR(("Position on path need to be somewhere on the path"));
			break;
		}
	}

	SetPathValidityStatus(ValidityIsBeingChecked);

	// ok, we perform the first step of path validation step
	// note that the scouting backWard is perform here once and for all as we do not need to go back to the start.

	KyResult validityBackwardResult = KY_SUCCESS;
	KyResult validityForwardResult = KY_SUCCESS;

	// just check if currentTarget is not the GetPath() Start node
	if (m_lowerBound.IsAtFirstNodeOfPath() == false)
	{
		validityBackwardResult = ValidateBackWard<NavTagTraversePredicate>(minDistanceFromTargetOnPathBackward, predicate);

		if (m_upperBound != m_lowerBound)
		{
			if (KY_FAILED(BuildEventListFromLowerBoundToUpperBound()))
			{
				// The build of the event list failed
				// we consider the validity interval to be invalid before target because we cannot build the event List
				SetPathValidityStatus(Dirty_InvalidBeforeTarget_ProcessingAfter);
				m_pathEventList.Clear();
				m_lowerBound = m_upperBound;
				m_lowerTrianglePtr = m_upperTrianglePtr;
				m_needToCheckLastEventForRemoval = false;
			}
			else
			{
				if (m_pathEventList.GetPathEventCount() > 1)
					// make the pathEventList in wanted configuration for the call to ValidateForward
					SetPathEventStatusForLastEvent(PathEventStatus_TemporaryUpperBound);
			}
		}
	}

	// just check if currentTarget is not the GetPath() End node
	if (m_upperBound.IsAtLastNodeOfPath() == false)
	{
		if (m_pathEventList.m_pathEventIntervalArray.GetCount() == 0)
		{
			// No pathEvent has been found, create 1 at targetOnPath
			PathEventInterval& firstEventOnPath = m_pathEventList.m_pathEventIntervalArray.PushDefault();
			firstEventOnPath.m_endingEventOnPath.m_positionOnPath = m_upperBound;
			firstEventOnPath.m_endingEventOnPath.m_eventStatusInList = PathEventStatus_LowerBound;
			PathEventType eventType = GetEventTypeFromLowerBound(m_upperBound);
			firstEventOnPath.m_endingEventOnPath.m_eventType = eventType;

			if (GetPathValidityStatus() != Dirty_InvalidBeforeTarget_ProcessingAfter && eventType == PathEventType_OnNavMeshAfterCollistion)
				firstEventOnPath.m_endingEventOnPath.m_eventType = PathEventType_OnNavMeshPathNode;
		}

		validityForwardResult = ValidateForward<NavTagTraversePredicate>(minDistanceFromTargetOnPathFrowaard, predicate);
	}
	else
	{
		if (GetPathValidityStatus() == Dirty_InvalidBeforeTarget_ProcessingAfter)
			SetPathValidityStatus(Dirty_InvalidBeforeTarget_ValidAfter);
		else
			SetPathValidityStatus(PathIsValid);

		if (m_pathEventList.GetPathEventCount() > 1)
			SetPathEventStatusForLastEvent(PathEventStatus_UpperBound);
	}

	// Deal with failure cases. It should not happen if the building stage run correctly but errors may occurs if the path is too close from wall for instance
	if (m_pathEventList.GetPathEventCount() <= 1)
	{
		SetPathValidityStatus(Dirty_TargetInvalid);
		ClearBoundsAndEventList();
		return KY_ERROR;
	}

	return validityBackwardResult == KY_SUCCESS && validityForwardResult == KY_SUCCESS ? KY_SUCCESS : KY_ERROR;
}

template<class NavTagTraversePredicate>
inline KyResult PathValidityInterval::ValidateBackWard(KyFloat32 minimumdrawBackDistance, NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_lowerBound.GetPath() != KY_NULL);
	KY_ASSERT(m_lowerBound.GetPath() == GetPath());
	KY_ASSERT(m_lowerBound.GetPath()->GetNodeCount() > 1);
	KY_ASSERT(m_lowerBound.IsAtFirstNodeOfPath() == false);

	KyFloat32 remainingDistance = minimumdrawBackDistance;

	for(;;)
	{
		const Vec3f previousPositon = m_lowerBound.GetPosition(); // Don't take reference here, we want to get its displacement distance

		if(KY_FAILED(TestCanGoToPrevPathNode<NavTagTraversePredicate>(predicate)))
		{
			FindLastValidPositionBackward<NavTagTraversePredicate>(predicate);
			return KY_ERROR;
		}

		remainingDistance -= Distance(previousPositon, m_lowerBound.GetPosition());

		if (m_lowerBound.IsAtFirstNodeOfPath())
			// we reach the start of the Path, we stop
			return KY_SUCCESS;

		if (remainingDistance < 0.f)
			// we move enough
			return KY_SUCCESS;
	}
}

template<class NavTagTraversePredicate>
inline KyResult PathValidityInterval::ValidateForward(KyFloat32 minimumDistanceAdvancement, NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_upperBound.GetPath() != KY_NULL);
	KY_ASSERT(m_upperBound.GetPath() == GetPath());
	KY_ASSERT(m_upperBound.IsAtLastNodeOfPath() == false);

	// Fix the status of the last event
	{
		PathEventInterval& lastInterval = m_pathEventList.m_pathEventIntervalArray.Back();
		PathEvent& lastEvent = lastInterval.m_endingEventOnPath;
		KY_LOG_ERROR_IF(lastEvent.m_eventStatusInList != PathEventStatus_TemporaryUpperBound && lastEvent.m_eventStatusInList != PathEventStatus_LowerBound, ("error of event status"));
		if (lastEvent.m_eventStatusInList == PathEventStatus_TemporaryUpperBound)
			lastEvent.m_eventStatusInList = PathEventStatus_WithinBounds;
	}

	KyFloat32 remainingDistance = minimumDistanceAdvancement;

	for(;;)
	{
		const Vec3f previousPositon = m_upperBound.GetPosition(); // Don't take reference here, we want to get its displacement distance

		if(KY_FAILED(TestCanGoToNextPathNode<NavTagTraversePredicate>(predicate)))
		{
			FindLastValidPositionForward<NavTagTraversePredicate>(predicate);

			// We are done with the validation
			if (m_pathEventList.GetPathEventCount() != 1)
				SetPathEventStatusForLastEvent(PathEventStatus_UpperBound);

			return KY_ERROR;
		}

		remainingDistance -= Distance(previousPositon, m_upperBound.GetPosition());

		if (m_upperBound.IsAtLastNodeOfPath())
		{
			// we reach the end of the Path, we stop

			if (GetPathValidityStatus() == Dirty_InvalidBeforeTarget_ProcessingAfter)
				SetPathValidityStatus(Dirty_InvalidBeforeTarget_ValidAfter);
			else
				SetPathValidityStatus(PathIsValid);

			// We are done with the validation
			SetPathEventStatusForLastEvent(PathEventStatus_UpperBound);
			return KY_SUCCESS;
		}

		if (remainingDistance <= 0.0f) // we move enough
		{
			// We are done for this validity stage, we did not reached then end of path yet, we tag the upperBound as temporary
			SetPathEventStatusForLastEvent(PathEventStatus_TemporaryUpperBound);
			return KY_SUCCESS;
		}
	}
}

template<class NavTagTraversePredicate>
inline KyResult PathValidityInterval::TestCanGoToPrevPathNode(NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_lowerBound.GetPath() != KY_NULL);
	KY_ASSERT(m_lowerBound.GetPath() == GetPath());
	KY_ASSERT(m_lowerBound.GetPath()->GetNodeCount() > 1);
	KY_ASSERT(m_lowerBound.IsAtFirstNodeOfPath() == false);

	const KyUInt32 pathEdgeIdxToMoveOn = m_lowerBound.GetEdgeIdxToMoveOnBackward();

	const Vec3f& edgeStart = GetPath()->GetPathEdgeStartPosition(pathEdgeIdxToMoveOn);
	const Vec3f& edgeEnd = GetPath()->GetPathEdgeEndPosition(pathEdgeIdxToMoveOn);

	PathEdgeType edgeType = GetPath()->GetPathEdgeType(pathEdgeIdxToMoveOn);

	switch (edgeType)
	{
	case PathEdgeType_OnNavMesh:
		{
			RayCanGoOnSegmentQuery<NavTagTraversePredicate> rayCanGoOnSegmentQuery(*predicate);
			rayCanGoOnSegmentQuery.Initialize(GetPath()->m_database, m_lowerBound.GetPosition(), m_lowerTrianglePtr, edgeEnd, edgeStart);
			rayCanGoOnSegmentQuery.SetDynamicOutputMode(QUERY_SAVE_NAVTAGSUBSEGMENTS);
			rayCanGoOnSegmentQuery.PerformQuery();

			if (rayCanGoOnSegmentQuery.GetResult() != RAYCANGOONSEGMENT_DONE_SUCCESS)
				return KY_ERROR;

			m_lowerTrianglePtr = rayCanGoOnSegmentQuery.GetArrivalTrianglePtr();
			break;
		}
	case PathEdgeType_OnNavGraph:
		{
			const NavGraphEdgePtr& graphEdgePtr = GetPath()->GetEdgeNavGraphEdgePtr(pathEdgeIdxToMoveOn);
			if (graphEdgePtr.IsValid() == false)
				return KY_ERROR;

			const NavGraphVertexRawPtr startGraphVertex = graphEdgePtr.GetRawPtr().GetStartNavGraphVertexRawPtr();
			const GraphVertexData& vertexData = startGraphVertex.GetGraphVertexData();

			if (vertexData.m_navGraphLink != KY_NULL && vertexData.m_navGraphLink->IsValid())
				m_lowerTrianglePtr = NavTrianglePtr(vertexData.m_navGraphLink->m_navTriangleRawPtr);
			else
				m_lowerTrianglePtr.Invalidate();
			break;
		}
	case PathEdgeType_FromOutside:
	case PathEdgeType_ToOutside:
		{
			m_lowerTrianglePtr.Invalidate();
			break;
		}
	default :
		{
			KY_LOG_ERROR(("Position on path need to be somewhere on the path"));
			break;
		}
	}

	m_lowerBound.MoveBackwardToPrevPathNode();
	return KY_SUCCESS;
}

template<class NavTagTraversePredicate>
inline KyResult PathValidityInterval::TestCanGoToNextPathNode(NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_upperBound.GetPath() != KY_NULL);
	KY_ASSERT(m_upperBound.GetPath() == GetPath());
	KY_ASSERT(m_upperBound.GetPath()->GetNodeCount() > 1);
	KY_ASSERT(m_upperBound.IsAtLastNodeOfPath() == false);

	const KyUInt32 pathEdgeIdxToMoveOn = m_upperBound.GetEdgeIdxToMoveOnForward();

	const Vec3f& edgeStart = GetPath()->GetPathEdgeStartPosition(pathEdgeIdxToMoveOn);
	const Vec3f& edgeEnd = GetPath()->GetPathEdgeEndPosition(pathEdgeIdxToMoveOn);

	PathEdgeType edgeType = GetPath()->GetPathEdgeType(pathEdgeIdxToMoveOn);

	switch (edgeType)
	{
	case PathEdgeType_OnNavMesh:
		{
			RayCanGoOnSegmentQuery<NavTagTraversePredicate> rayCanGoOnSegmentQuery(*predicate);
			rayCanGoOnSegmentQuery.Initialize(GetPath()->m_database, m_upperBound.GetPosition(), m_upperTrianglePtr, edgeStart, edgeEnd);
			rayCanGoOnSegmentQuery.SetDynamicOutputMode(QUERY_SAVE_NAVTAGSUBSEGMENTS);
			rayCanGoOnSegmentQuery.SetQueryDynamicOutput(m_queryDynamicOutput);
			rayCanGoOnSegmentQuery.PerformQuery();

			if (rayCanGoOnSegmentQuery.GetResult() != RAYCANGOONSEGMENT_DONE_SUCCESS)
				return KY_ERROR;

			// there is at least one stag subSegment
			m_queryDynamicOutput = rayCanGoOnSegmentQuery.GetQueryDynamicOutput();
			KY_LOG_ERROR_IF(m_queryDynamicOutput->GetNavTagSubSegment(m_queryDynamicOutput->GetNavTagSubSegmentCount() - 1).m_exitPos3f.Get2d() != edgeEnd.Get2d(), ("This should not happen"));

			CreateIntervalForNavTagSubSegment(pathEdgeIdxToMoveOn);

			m_upperTrianglePtr = rayCanGoOnSegmentQuery.GetArrivalTrianglePtr();
			break;
		}
	case PathEdgeType_OnNavGraph:
		{
			m_needToCheckLastEventForRemoval = false;
			const NavGraphEdgePtr& graphEdgePtr = GetPath()->GetEdgeNavGraphEdgePtr(pathEdgeIdxToMoveOn);

			if (graphEdgePtr.IsValid() == false)
				return KY_ERROR;

			CreateIntervalForGraphEdge(pathEdgeIdxToMoveOn);

			const NavGraphVertexRawPtr endGraphVertex = graphEdgePtr.GetRawPtr().GetEndNavGraphVertexRawPtr();
			const GraphVertexData& vertexData = endGraphVertex.GetGraphVertexData();

			if (vertexData.m_navGraphLink != KY_NULL && vertexData.m_navGraphLink->IsValid())
				m_upperTrianglePtr = NavTrianglePtr(vertexData.m_navGraphLink->m_navTriangleRawPtr);
			else
				m_upperTrianglePtr.Invalidate();
			break;
		}
	case PathEdgeType_FromOutside:
		{
			m_needToCheckLastEventForRemoval = false;
			CreateIntervalForFromOutsidePathEdge(pathEdgeIdxToMoveOn);
			m_upperTrianglePtr = GetPath()->GetNodeNavTrianglePtr(pathEdgeIdxToMoveOn + 1);
			break;
		}

	case PathEdgeType_ToOutside:
		{
			m_needToCheckLastEventForRemoval = false;
			CreateIntervalForToOutsidePathEdge(pathEdgeIdxToMoveOn);
			m_upperTrianglePtr.Invalidate();
			break;
		}

	default:
		KY_LOG_ERROR(("all edges on the path must have a defined type"));
		break;
	}

	m_upperBound.MoveForwardToNextPathNode();
	return KY_SUCCESS;
}


template<class NavTagTraversePredicate>
inline void PathValidityInterval::FindLastValidPositionForward(NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_upperBound.GetPath() != KY_NULL);
	KY_ASSERT(m_upperBound.GetPath() == GetPath());
	KY_ASSERT(m_upperBound.IsAtLastNodeOfPath() == false);

	if (GetPathValidityStatus() == Dirty_InvalidBeforeTarget_ProcessingAfter)
		SetPathValidityStatus(Dirty_InvalidBeforeAndAfterTarget);
	else
		SetPathValidityStatus(Dirty_ValidBeforeTarget_InvalidAfterTarget);

	const KyUInt32 pathEdgeIdxToMoveOn = m_upperBound.GetEdgeIdxToMoveOnForward();

	if (GetPath()->GetPathEdgeType(pathEdgeIdxToMoveOn) != PathEdgeType_OnNavMesh)
		return;

	if (m_upperTrianglePtr.IsValid() == false)
		return;

	const Vec3f& edgeStart = GetPath()->GetPathEdgeStartPosition(pathEdgeIdxToMoveOn);
	const Vec3f& edgeEnd = GetPath()->GetPathEdgeEndPosition(pathEdgeIdxToMoveOn);
	const KyFloat32 castDistance = Distance2d(m_upperBound.GetPosition(), edgeEnd);

	RayCastOnSegmentQuery<NavTagTraversePredicate> rayCastOnSegmentQuery(*predicate);
	rayCastOnSegmentQuery.Initialize(GetPath()->m_database, m_upperBound.GetPosition(), m_upperTrianglePtr, edgeStart, edgeEnd, castDistance);
	rayCastOnSegmentQuery.SetDynamicOutputMode(QUERY_SAVE_NAVTAGSUBSEGMENTS);
	rayCastOnSegmentQuery.SetQueryDynamicOutput(m_queryDynamicOutput);
	rayCastOnSegmentQuery.PerformQuery();

	if ((rayCastOnSegmentQuery.GetResult() == RAYCASTONSEGMENT_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED) || 
		(rayCastOnSegmentQuery.GetResult() == RAYCASTONSEGMENT_DONE_ARRIVALPOS_FOUND_AFTER_COLLISION))
	{
		m_queryDynamicOutput = rayCastOnSegmentQuery.GetQueryDynamicOutput();
		CreateIntervalsForNavTagSubsegmentButLast(pathEdgeIdxToMoveOn);
		CreateIntervalForLastNavTagSubSegment(pathEdgeIdxToMoveOn, PathEventType_OnNavMeshAfterCollistion);

		m_upperBound.m_onPathStatus = PositionOnPathStatus_OnPathEdge;
		m_upperBound.m_currentIndexOnPath = pathEdgeIdxToMoveOn;
		m_upperTrianglePtr = rayCastOnSegmentQuery.GetArrivalTrianglePtr();
		m_upperBound.m_position = rayCastOnSegmentQuery.GetArrivalPos();

		PathEventInterval& lastInterval = m_pathEventList.m_pathEventIntervalArray.Back();
		PathEvent& lastEvent = lastInterval.m_endingEventOnPath;
		lastEvent.m_positionOnPath = m_upperBound;
		m_needToCheckLastEventForRemoval = false;
	}
}

template<class NavTagTraversePredicate>
inline void PathValidityInterval::FindLastValidPositionBackward(NavTagTraversePredicate* predicate)
{
	KY_ASSERT(m_lowerBound.GetPath() != KY_NULL);
	KY_ASSERT(m_lowerBound.GetPath() == GetPath());
	KY_ASSERT(m_lowerBound.IsAtFirstNodeOfPath() == false);

	SetPathValidityStatus(Dirty_InvalidBeforeTarget_ProcessingAfter);

	const KyUInt32 pathEdgeIdxToMoveOn = m_lowerBound.GetEdgeIdxToMoveOnBackward();

	if (GetPath()->GetPathEdgeType(pathEdgeIdxToMoveOn) != PathEdgeType_OnNavMesh)
		return;

	if (m_lowerTrianglePtr.IsValid() == false)
		return;

	const Vec3f& edgeStart = GetPath()->GetPathEdgeStartPosition(pathEdgeIdxToMoveOn);
	const Vec3f& edgeEnd = GetPath()->GetPathEdgeEndPosition(pathEdgeIdxToMoveOn);
	const KyFloat32 castDistance = Distance2d(m_lowerBound.GetPosition(), edgeStart);

	RayCastOnSegmentQuery<NavTagTraversePredicate> rayCastOnSegmentQuery(*predicate);
	rayCastOnSegmentQuery.Initialize(GetPath()->m_database, m_lowerBound.GetPosition(), m_lowerTrianglePtr, edgeEnd, edgeStart, castDistance);
	rayCastOnSegmentQuery.PerformQuery();

	if ((rayCastOnSegmentQuery.GetResult() == RAYCASTONSEGMENT_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED) || 
		(rayCastOnSegmentQuery.GetResult() == RAYCASTONSEGMENT_DONE_ARRIVALPOS_FOUND_AFTER_COLLISION))
	{
		m_lowerBound.m_onPathStatus = PositionOnPathStatus_OnPathEdge;
		m_lowerBound.m_currentIndexOnPath = pathEdgeIdxToMoveOn;
		m_lowerTrianglePtr = rayCastOnSegmentQuery.GetArrivalTrianglePtr();
		m_lowerBound.m_position = rayCastOnSegmentQuery.GetArrivalPos();
	}
}


}

