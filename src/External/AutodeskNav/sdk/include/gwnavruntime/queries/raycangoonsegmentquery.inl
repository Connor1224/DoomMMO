/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

template<class NavTagTraversePredicate>
KY_INLINE RayCanGoOnSegmentQuery<NavTagTraversePredicate>::RayCanGoOnSegmentQuery() : BaseRayCanGoOnSegmentQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE RayCanGoOnSegmentQuery<NavTagTraversePredicate>::RayCanGoOnSegmentQuery(const NavTagTraversePredicate& predicate) :
	BaseRayCanGoOnSegmentQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& startPos, const NavTrianglePtr& startTrianglePtr,
	const Vec3f& segmentStartPos, const Vec3f& segmentEndPos)
{
	BaseRayCanGoOnSegmentQuery::Initialize(database, startPos, startTrianglePtr, segmentStartPos, segmentEndPos);
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseRayCanGoOnSegmentQuery::SetPositionSpatializationRange(positionSpatializationRange);
}
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)
{
	BaseRayCanGoOnSegmentQuery::SetDynamicOutputMode(savingMode);
}
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)
{
	BaseRayCanGoOnSegmentQuery::SetQueryDynamicOutput(queryDynamicOutput);
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)
{
	BaseRayCanGoOnSegmentQuery::SetStartIntegerPos(startIntegerPos);
}
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetSegmentStartIntegerPos(const WorldIntegerPos& segmentStartIntegerPos)
{
	BaseRayCanGoOnSegmentQuery::SetSegmentStartIntegerPos(segmentStartIntegerPos);
}
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::SetSegmentEndIntegerPos(const WorldIntegerPos& segmentEndIntegerPos)
{
	BaseRayCanGoOnSegmentQuery::SetSegmentEndIntegerPos(segmentEndIntegerPos);
}

template<class NavTagTraversePredicate>
KY_INLINE RayCanGoOnSegmentQueryResult RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetResult()                 const { return BaseRayCanGoOnSegmentQuery::GetResult();                 }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode            RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetDynamicOutputMode()      const { return BaseRayCanGoOnSegmentQuery::GetDynamicOutputMode();      }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&                 RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetStartPos()               const { return BaseRayCanGoOnSegmentQuery::GetStartPos();               }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&                 RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetSegmentStartPos()        const { return BaseRayCanGoOnSegmentQuery::GetSegmentStartPos();        }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&                 RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetSegmentEndPos()          const { return BaseRayCanGoOnSegmentQuery::GetSegmentEndPos();          }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&        RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetStartTrianglePtr()       const { return BaseRayCanGoOnSegmentQuery::GetStartTrianglePtr();       }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&        RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetArrivalTrianglePtr()     const { return BaseRayCanGoOnSegmentQuery::GetArrivalTrianglePtr();     }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*          RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()     const { return BaseRayCanGoOnSegmentQuery::GetQueryDynamicOutput();     }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&       RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetStartIntegerPos()        const { return BaseRayCanGoOnSegmentQuery::GetStartIntegerPos();        }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&       RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetSegmentStartIntegerPos() const { return BaseRayCanGoOnSegmentQuery::GetSegmentStartIntegerPos(); }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&       RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetSegmentEndIntegerPos()   const { return BaseRayCanGoOnSegmentQuery::GetSegmentEndIntegerPos();   }

template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& RayCanGoOnSegmentQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const
{
	return BaseRayCanGoOnSegmentQuery::GetPositionSpatializationRange();
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != RAYCANGOONSEGMENT_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(RAYCANGOONSEGMENT_DONE_START_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_startPos3f, m_startIntegerPos);
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_segmentStartPos3f, m_segmentStartIntegerPos);
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_segmentEndPos3f, m_segmentEndIntegerPos);

	PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
void RayCanGoOnSegmentQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == RAYCANGOONSEGMENT_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	KyFloat32 integerPrecision = databaseGenMetrics.m_integerPrecision;

	WorkingMemArray<NavTriangleRawPtr> crossedTriangles;
	WorkingMemArray<RawNavTagSubSegment> navTagSubSegments;

	Vec2f normalizedDir2d(m_segmentEndPos3f.x - m_segmentStartPos3f.x, m_segmentEndPos3f.y - m_segmentStartPos3f.y);
	normalizedDir2d.Normalize();

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_startIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(RAYCANGOONSEGMENT_DONE_START_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr startTriangleRawPtr = m_startTrianglePtr.GetRawPtr();

	if (m_predicate.CanTraverse(startTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(RAYCANGOONSEGMENT_DONE_START_NAVTAG_FORBIDDEN);
		return;
	}

	ScopeAutoSaveDynamicOutput scopeAutoSaveDynOutput(m_queryDynamicOutput);
	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		crossedTriangles.Init(workingMemory);
		if (crossedTriangles.IsInitialized() == false)
		{
			SetResult(RAYCANGOONSEGMENT_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&crossedTriangles);

		if (KY_FAILED(crossedTriangles.PushBack(startTriangleRawPtr)))
		{
			KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
			SetResult(RAYCANGOONSEGMENT_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
	{
		navTagSubSegments.Init(workingMemory);
		if (navTagSubSegments.IsInitialized() == false)
		{
			SetResult(RAYCANGOONSEGMENT_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetRawNavTagSubSegments(&navTagSubSegments);
	}

	const Vec3f& destPos3f = m_segmentEndPos3f;

	RayQueryUtils rayQueryUtils(m_startPos3f, destPos3f, GetDynamicOutputMode(), &crossedTriangles, &navTagSubSegments);
	const bool storePropagationData =
		(GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES       ) != 0 ||
		(GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0;

#if defined (KY_BUILD_DEBUG)
	const CellPos minCellPos(Min(m_segmentStartIntegerPos.m_cellPos.x, m_segmentEndIntegerPos.m_cellPos.x), Min(m_segmentStartIntegerPos.m_cellPos.y, m_segmentEndIntegerPos.m_cellPos.y));
	const CellPos maxCellPos(Max(m_segmentStartIntegerPos.m_cellPos.x, m_segmentEndIntegerPos.m_cellPos.x) + 1, Max(m_segmentStartIntegerPos.m_cellPos.y, m_segmentEndIntegerPos.m_cellPos.y) + 1);
	const CoordBox64 cellsIntegerBox(databaseGenMetrics.ComputeCellOrigin(minCellPos), databaseGenMetrics.ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 20))
	{
		KY_LOG_WARNING( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
		SetResult(RAYCANGOONSEGMENT_DONE_QUERY_TOO_LONG);
		return;
	}
#endif

	RawNavTagSubSegment currentNavTagSubSegment;
	currentNavTagSubSegment.m_navTriangleRawPtr = startTriangleRawPtr;
	currentNavTagSubSegment.m_entrancePos3f = m_startPos3f;

	const CoordPos64 segmentStartCoordPos64 = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_segmentStartIntegerPos);
	const CoordPos64 segmentEndCoordPos64   = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_segmentEndIntegerPos);

	const CoordPos64 direction(segmentEndCoordPos64 - segmentStartCoordPos64);
	const KyInt64 startToEndquareDist = DotProduct(direction, direction);

	KyInt32 edgeNumberToCrossInTriangle = rayQueryUtils.GetfirstEdgeToCrossInStartTriangle(startTriangleRawPtr, segmentStartCoordPos64, segmentEndCoordPos64);
	const NavHalfEdgeIdx firstHaldEdgeIdxToCross = NavFloorBlob::NavTriangleIdxToNavHalfEdgeIdx(startTriangleRawPtr.GetTriangleIdx(), edgeNumberToCrossInTriangle);

	NavHalfEdgeRawPtr currentHalfEdgeRawPtrToCross(startTriangleRawPtr.m_navFloorRawPtr, firstHaldEdgeIdxToCross);

	NavFloor* statingNavFloor = startTriangleRawPtr.GetNavFloor();
	CoordPos64 currentCellOrigin = databaseGenMetrics.ComputeCellOrigin(statingNavFloor->GetCellPos());
	const NavFloorBlob* currentNavFloorBlob = statingNavFloor->GetNavFloorBlob();
	NavHalfEdge currentNavHalfEdge = currentNavFloorBlob->GetNavHalfEdge(firstHaldEdgeIdxToCross);

	// Main loop
	// we check if we are not about to go beyond the point if we go farther than the current triangle
	// i.e. we check if dest is beyong the edge we are about to cross
	NavHalfEdgeRawPtr currentPairHalfEdgeRawPtr;
	while (rayQueryUtils.IsEdgeBeyondDestPos(currentCellOrigin, *currentNavFloorBlob, currentHalfEdgeRawPtrToCross.GetHalfEdgeIdx(),
		segmentStartCoordPos64, direction, segmentEndCoordPos64, startToEndquareDist) == false)
	{
		// ok, we can try to cross the edge
		if(currentHalfEdgeRawPtrToCross.IsHalfEdgeCrossable<NavTagTraversePredicate>(currentNavHalfEdge, currentNavFloorBlob, currentPairHalfEdgeRawPtr, &m_predicate))
		{
			// ok we can cross the edge and pass in the other triangle
			if (currentNavHalfEdge.GetHalfEdgeType() != EDGETYPE_PAIRED)
			{
				NavFloor* currentNavFloor = currentPairHalfEdgeRawPtr.GetNavFloor();
				if (IsHalfEdgeACellBoundary(currentNavHalfEdge.GetHalfEdgeType()))
					currentCellOrigin = databaseGenMetrics.ComputeCellOrigin(currentNavFloor->GetCellPos());

				currentNavFloorBlob = currentNavFloor->GetNavFloorBlob();
			}

			if (storePropagationData)
			{
				if (KY_FAILED(rayQueryUtils.StorePropagationData(integerPrecision, currentPairHalfEdgeRawPtr, currentNavTagSubSegment,
					currentCellOrigin, currentNavFloorBlob)))
				{
					KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
					SetResult(RAYCANGOONSEGMENT_DONE_LACK_OF_WORKING_MEMORY);
					return;
				}
			}

			// before looping, we compute which edge we will cross in the triangle
			// for that, we look on which side of the direction the third vertex of the triangle is.
			const NavVertex& thirdVertexPosInFloor = currentNavFloorBlob->NavHalfEdgeIdxToThirdNavVertex(currentPairHalfEdgeRawPtr.GetHalfEdgeIdx());
			const CoordPos64 thirdVertexPos = currentCellOrigin + thirdVertexPosInFloor.GetCoordPos64();
			const KyInt64 crossProductToChooseTheGoodEdge = CrossProduct(thirdVertexPos - segmentStartCoordPos64, direction);

			if (crossProductToChooseTheGoodEdge > 0)
				// go on the left of third
				// currentHalfEdgeSafeIdToCross = currentPairHalfEdgeSafeId.GetPrevHalfEdgeSafeId();
				currentPairHalfEdgeRawPtr.GetPrevHalfEdgeRawPtr(currentHalfEdgeRawPtrToCross);
			else
				// go on the right of third
				// currentHalfEdgeSafeIdToCross = currentPairHalfEdgeSafeId.GetNextHalfEdge();
				currentPairHalfEdgeRawPtr.GetNextHalfEdgeRawPtr(currentHalfEdgeRawPtrToCross);

			currentNavHalfEdge = currentNavFloorBlob->GetNavHalfEdge(currentHalfEdgeRawPtrToCross.GetHalfEdgeIdx());

			continue;
		}

		// outch ! we hit a border, we stop !
		SetResult(RAYCANGOONSEGMENT_DONE_COLLISION_DETECTED);
		return;
	}

	RayCanGo_NoHit(currentHalfEdgeRawPtrToCross, currentNavTagSubSegment, navTagSubSegments);
}




}
