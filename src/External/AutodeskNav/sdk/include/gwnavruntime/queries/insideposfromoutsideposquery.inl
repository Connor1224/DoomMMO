/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

template<class NavTagTraversePredicate>
KY_INLINE InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::InsidePosFromOutsidePosQuery() : BaseInsidePosFromOutsidePosQuery(), m_predicate() {}

template<class NavTagTraversePredicate>
KY_INLINE InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::InsidePosFromOutsidePosQuery(const NavTagTraversePredicate& predicate) :
BaseInsidePosFromOutsidePosQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& inputPos)
{
	BaseInsidePosFromOutsidePosQuery::Initialize(database, inputPos);
}
template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::SetHorizontalTolerance(KyFloat32 horizontalTolerance)
{
	BaseInsidePosFromOutsidePosQuery::SetHorizontalTolerance(horizontalTolerance);
}
template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseInsidePosFromOutsidePosQuery::SetPositionSpatializationRange(positionSpatializationRange);
}
template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::SetDistFromObstacle(KyFloat32 distFromObstacle)
{
	BaseInsidePosFromOutsidePosQuery::SetDistFromObstacle(distFromObstacle);
}
template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::SetInputIntegerPos(const WorldIntegerPos& inputIntegerPos)
{
	BaseInsidePosFromOutsidePosQuery::SetInputIntegerPos(inputIntegerPos);
}
template<class NavTagTraversePredicate>
KY_INLINE InsidePosFromOutsidePosQueryResult InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetResult() const
{
	return BaseInsidePosFromOutsidePosQuery::GetResult();
}
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetInputPos() const
{
	return BaseInsidePosFromOutsidePosQuery::GetInputPos();
}
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetInsidePos() const
{
	return BaseInsidePosFromOutsidePosQuery::GetInsidePos();
}
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetInsidePosTrianglePtr() const
{
	return BaseInsidePosFromOutsidePosQuery::GetInsidePosTrianglePtr();
}
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32 InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetHorizontalTolerance() const
{
	return BaseInsidePosFromOutsidePosQuery::GetHorizontalTolerance();
}
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const
{
	return BaseInsidePosFromOutsidePosQuery::GetPositionSpatializationRange();
}
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32 InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetDistFromObstacle() const
{
	return BaseInsidePosFromOutsidePosQuery::GetDistFromObstacle();
}
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetInputIntegerPos() const
{
	return BaseInsidePosFromOutsidePosQuery::GetInputIntegerPos();
}
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::GetInsideIntegerPos() const
{
	return BaseInsidePosFromOutsidePosQuery::GetInsideIntegerPos();
}

template<class NavTagTraversePredicate>
KY_INLINE void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}


template<class NavTagTraversePredicate>
inline void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	KY_SCOPED_PERF_MARKER("Navigation InsidePosFromOutsidePosQuery");

	if (GetResult() != INSIDEPOSFROMOUTSIDE_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(INSIDEPOSFROMOUTSIDE_DONE_POS_NOT_FOUND);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_inputPos3f, m_inputIntegerPos);

	return PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
inline void InsidePosFromOutsidePosQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	NearestBorderHalfEdgeFromPosQuery<NavTagTraversePredicate> nearestBorderHalfEdgeFromPosQuery(m_predicate);
	nearestBorderHalfEdgeFromPosQuery.Initialize(m_database, m_inputPos3f);
	nearestBorderHalfEdgeFromPosQuery.SetHorizontalTolerance(m_horizontalTolerance);
	nearestBorderHalfEdgeFromPosQuery.SetPositionSpatializationRange(m_positionSpatializationRange);
	nearestBorderHalfEdgeFromPosQuery.SetInputIntegerPos(m_inputIntegerPos);

	nearestBorderHalfEdgeFromPosQuery.PerformQueryWithInputCoordPos(workingMemory);

	Vec3f startToEndNormalInside2d;

	if (KY_FAILED(ProcessNearestBorderHalfEdgeFromPosQueryResult(workingMemory, nearestBorderHalfEdgeFromPosQuery, startToEndNormalInside2d)))
		return;

	const NavHalfEdgeRawPtr nearestBorderHalfEdgeRawPtr = nearestBorderHalfEdgeFromPosQuery.GetNearestHalfEdgePtrOnBorder().GetRawPtr();

	NavTrianglePtr startTrianglePtr(nearestBorderHalfEdgeRawPtr.GetNavFloor(), NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(nearestBorderHalfEdgeRawPtr.GetHalfEdgeIdx()));

	RayCastQuery<NavTagTraversePredicate> rayCastQuery(m_predicate);
	rayCastQuery.Initialize(m_database, nearestBorderHalfEdgeFromPosQuery.GetNearestPosOnHalfEdge(), Vec2f(startToEndNormalInside2d.x, startToEndNormalInside2d.y), m_distFromObstacle);
	rayCastQuery.SetStartTrianglePtr(startTrianglePtr);
	rayCastQuery.SetStartIntegerPos(nearestBorderHalfEdgeFromPosQuery.GetNearestIntegerPosOnHalfEdge());
	
	rayCastQuery.PerformQueryWithInputCoordPos(workingMemory);

	const RayCastQueryResult rayCastQueryResult = rayCastQuery.GetResult();

	if (rayCastQueryResult == RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED || rayCastQueryResult == RAYCAST_DONE_ARRIVALPOS_FOUND_AFTER_COLLISION)
	{
		SetResult(INSIDEPOSFROMOUTSIDE_DONE_POS_FOUND);
		m_insidePos3f = rayCastQuery.GetArrivalPos();
		m_insidePosTrianglePtr = rayCastQuery.GetArrivalTrianglePtr();
	}
	else
	{
		SetResult(INSIDEPOSFROMOUTSIDE_DONE_POS_NOT_FOUND);
	}
}
}
