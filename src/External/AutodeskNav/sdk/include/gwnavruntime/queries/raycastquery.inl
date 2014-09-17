/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{
template<class NavTagTraversePredicate>
KY_INLINE RayCastQuery<NavTagTraversePredicate>::RayCastQuery() : BaseRayCastQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE RayCastQuery<NavTagTraversePredicate>::RayCastQuery(const NavTagTraversePredicate& predicate) : BaseRayCastQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& startPos, const Vec2f& normalizedDir2d, KyFloat32 maxDist)
{
	BaseRayCastQuery::Initialize(database, startPos, normalizedDir2d, maxDist);
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr) { BaseRayCastQuery::SetStartTrianglePtr(startTrianglePtr);   }
template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)  { BaseRayCastQuery::SetStartIntegerPos(startIntegerPos);     }
template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)          { BaseRayCastQuery::SetDynamicOutputMode(savingMode);        }
template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)
{
	BaseRayCastQuery::SetQueryDynamicOutput(queryDynamicOutput);
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseRayCastQuery::SetPositionSpatializationRange(positionSpatializationRange);
}

template<class NavTagTraversePredicate>
KY_INLINE RayCastQueryResult     RayCastQuery<NavTagTraversePredicate>::GetResult ()                 const { return BaseRayCastQuery::GetResult ();                 }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode      RayCastQuery<NavTagTraversePredicate>::GetDynamicOutputMode()       const { return BaseRayCastQuery::GetDynamicOutputMode();       }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&           RayCastQuery<NavTagTraversePredicate>::GetStartPos()                const { return BaseRayCastQuery::GetStartPos();                }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&           RayCastQuery<NavTagTraversePredicate>::GetCollisionPos()            const { return BaseRayCastQuery::GetCollisionPos();            }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&           RayCastQuery<NavTagTraversePredicate>::GetArrivalPos()              const { return BaseRayCastQuery::GetArrivalPos();              }
template<class NavTagTraversePredicate>
KY_INLINE const Vec2f&           RayCastQuery<NavTagTraversePredicate>::GetNormalizedDir2d()         const { return BaseRayCastQuery::GetNormalizedDir2d();         }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&  RayCastQuery<NavTagTraversePredicate>::GetStartTrianglePtr()        const { return BaseRayCastQuery::GetStartTrianglePtr();        }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&  RayCastQuery<NavTagTraversePredicate>::GetArrivalTrianglePtr()      const { return BaseRayCastQuery::GetArrivalTrianglePtr();      }
template<class NavTagTraversePredicate>
KY_INLINE const NavHalfEdgePtr&  RayCastQuery<NavTagTraversePredicate>::GetCollisionNavHalfEdgePtr() const { return BaseRayCastQuery::GetCollisionNavHalfEdgePtr(); }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32              RayCastQuery<NavTagTraversePredicate>::GetMaxDist()                 const { return BaseRayCastQuery::GetMaxDist();                 }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*    RayCastQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()      const { return BaseRayCastQuery::GetQueryDynamicOutput();      }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& RayCastQuery<NavTagTraversePredicate>::GetStartIntegerPos()         const { return BaseRayCastQuery::GetStartIntegerPos();         }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& RayCastQuery<NavTagTraversePredicate>::GetArrivalIntegerPos()       const { return BaseRayCastQuery::GetArrivalIntegerPos();       }
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& RayCastQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const
{
	return BaseRayCastQuery::GetPositionSpatializationRange();
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCastQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
inline void RayCastQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != RAYCAST_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(RAYCAST_DONE_START_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_startPos3f, m_startIntegerPos);

	PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
inline void RayCastQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == RAYCAST_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	KyFloat32 integerPrecision = databaseGenMetrics.m_integerPrecision;

	WorkingMemArray<NavTriangleRawPtr> crossedTriangles;
	WorkingMemArray<RawNavTagSubSegment> navTagSubSegments;

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_startIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(RAYCAST_DONE_START_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr startTriangleRawPtr = m_startTrianglePtr.GetRawPtr();

	if(m_predicate.CanTraverse(startTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(RAYCAST_DONE_START_NAVTAG_FORBIDDEN);
		return;
	}

	ScopeAutoSaveDynamicOutput scopeAutoSaveDynOutput(m_queryDynamicOutput);
	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		crossedTriangles.Init(workingMemory);
		if (crossedTriangles.IsInitialized() == false)
		{
			SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&crossedTriangles);

		if (KY_FAILED(crossedTriangles.PushBack(startTriangleRawPtr)))
		{
			KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
			SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
	{
		navTagSubSegments.Init(workingMemory);
		if (navTagSubSegments.IsInitialized() == false)
		{
			SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetRawNavTagSubSegments(&navTagSubSegments);
	}

	const Vec2f direction2f(m_normalizedDir2d * m_maxDist);
	Vec3f destPos3f(m_startPos3f.x + direction2f.x, m_startPos3f.y + direction2f.y, m_startPos3f.z);
	const WorldIntegerPos destIntegerPos(databaseGenMetrics.GetWorldIntegerPosFromVec3f(destPos3f));

	RayQueryUtils rayQueryUtils(m_startPos3f, destPos3f, GetDynamicOutputMode(), &crossedTriangles, &navTagSubSegments);

	const bool storePropagationData =
		(GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES       ) != 0 ||
		(GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0;

	if (destIntegerPos == m_startIntegerPos)
	{
		// Nothing to do
		m_arrivalPos3f = m_startPos3f;
		m_arrivalIntegerPos = m_startIntegerPos;
		m_arrivalTrianglePtr = m_startTrianglePtr;
		SetResult(RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED);
		return;
	}

	RawNavTagSubSegment currentNavTagSubSegment;
	currentNavTagSubSegment.m_navTriangleRawPtr = startTriangleRawPtr;
	currentNavTagSubSegment.m_entrancePos3f = m_startPos3f;

	const CellPos minCellPos(Min(m_startIntegerPos.m_cellPos.x, destIntegerPos.m_cellPos.x), Min(m_startIntegerPos.m_cellPos.y, destIntegerPos.m_cellPos.y));
	const CellPos maxCellPos(Max(m_startIntegerPos.m_cellPos.x, destIntegerPos.m_cellPos.x) + 1, Max(m_startIntegerPos.m_cellPos.y, destIntegerPos.m_cellPos.y) + 1);
	const CoordBox64 cellsIntegerBox(databaseGenMetrics.ComputeCellOrigin(minCellPos), databaseGenMetrics.ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 20))
	{
		KY_LOG_WARNING( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
		SetResult(RAYCAST_DONE_QUERY_TOO_LONG);
		return;
	}

	const CoordPos64 startCoordPos64 = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_startIntegerPos);
	const CoordPos64 destCoordPos64 = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(destIntegerPos);

	const CoordPos64 direction(destCoordPos64 - startCoordPos64);
	const KyInt64 startToDestSquareDist = DotProduct(direction, direction);

	KyInt32 edgeNumberToCrossInTriangle = rayQueryUtils.GetfirstEdgeToCrossInStartTriangle(startTriangleRawPtr, startCoordPos64, destCoordPos64);
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
		startCoordPos64, direction, destCoordPos64, startToDestSquareDist) == false)
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
					SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
					return;
				}
			}

			// before looping, we compute which edge we will cross in the triangle
			// for that, we look on which side of the direction the third vertex of the triangle is.
			const NavVertex& thirdVertexPosInFloor = currentNavFloorBlob->NavHalfEdgeIdxToThirdNavVertex(currentPairHalfEdgeRawPtr.GetHalfEdgeIdx());
			const CoordPos64 thirdVertexPos = currentCellOrigin + thirdVertexPosInFloor.GetCoordPos64();
			const KyInt64 crossProductToChooseTheGoodEdge = CrossProduct(thirdVertexPos - startCoordPos64, direction);

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
		RayCast_BorderHit(queryUtils, currentHalfEdgeRawPtrToCross, destCoordPos64, currentNavTagSubSegment, crossedTriangles, navTagSubSegments);

		return;
	}

	RayCast_NoHit(currentHalfEdgeRawPtrToCross, destIntegerPos, destPos3f, currentNavTagSubSegment, navTagSubSegments);
}

template<class NavTagTraversePredicate>
inline void RayCastQuery<NavTagTraversePredicate>::RayCast_BorderHit(QueryUtils& queryUtils, const NavHalfEdgeRawPtr& halfEdgeIdRawPtrHit, const CoordPos64& destCoordPos64,
	RawNavTagSubSegment& navTagSubSegment, WorkingMemArray<NavTriangleRawPtr>& crossedTriangles, WorkingMemArray<RawNavTagSubSegment>& navTagSubSegments)
{
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	const KyFloat32 integerPrecision = databaseGenMetrics.m_integerPrecision;

	const CoordPos64 startCoordPos64 = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_startIntegerPos);

	// before return, we retrieve the edge hit and we move back the collision point
	// the half-edge hit is defined by currentTrianglePtr and edgeToCross
	m_collisionHalfEdgePtr.Set(halfEdgeIdRawPtrHit.GetNavFloor(), halfEdgeIdRawPtrHit.GetHalfEdgeIdx());

	Vec3f edgeStartPos3f, edgeEndPos3f, edgeThirdPos3f;
	CoordPos64 collisionCoordPos64;
	ComputeCollisionPosition(halfEdgeIdRawPtrHit, startCoordPos64, destCoordPos64, collisionCoordPos64, edgeStartPos3f, edgeEndPos3f, edgeThirdPos3f);

	if (KY_FAILED(queryUtils.FindValidPositionFromIntegerIntersection<NavTagTraversePredicate>(m_collisionPos3f, m_collisionHalfEdgePtr, m_arrivalIntegerPos,
		m_arrivalTrianglePtr, &m_predicate)))
	{
		// if failed, m_arrivalIntegerPos did not changed
		m_arrivalPos3f = m_collisionPos3f;

		if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
		{
			navTagSubSegment.m_exitPos3f = m_collisionPos3f;
			if(KY_FAILED(navTagSubSegments.PushBack(navTagSubSegment)))
			{
				KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
				SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
				return;
			}
		}

		SetResult(RAYCAST_DONE_COLLISION_FOUND_ARRIVAL_ERROR);
		return;
	}

	CoordPos64 arrivalCoordPos64 = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_arrivalIntegerPos);

	m_arrivalPos3f.x = arrivalCoordPos64.x * integerPrecision;
	m_arrivalPos3f.y = arrivalCoordPos64.y * integerPrecision;
	m_arrivalPos3f.z = Intersections::ComputeAltitudeOfPointInTriangle(m_collisionPos3f, edgeStartPos3f, edgeEndPos3f, edgeThirdPos3f);

	const KyInt64 dotProduct2dToNotGoBackBeforeStart = DotProduct(destCoordPos64 - startCoordPos64, arrivalCoordPos64 - startCoordPos64);

	if (dotProduct2dToNotGoBackBeforeStart < 0)
	{
		//we move back beyond m_startPos, which is bad : we return StartPos
		if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
		{
			navTagSubSegment.m_exitPos3f = m_startPos3f;
			if(KY_FAILED(navTagSubSegments.PushBack(navTagSubSegment)))
			{
				KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
				SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
				return;
			}
		}

		m_arrivalPos3f = m_startPos3f;
		m_arrivalIntegerPos = m_startIntegerPos;
		m_arrivalTrianglePtr = m_startTrianglePtr;
		SetResult(RAYCAST_DONE_CANNOT_MOVE);
		return;
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		if (m_arrivalTrianglePtr.m_navFloorPtr.GetNavFloor_Unsafe() != halfEdgeIdRawPtrHit.m_navFloorRawPtr.GetNavFloor() ||
			m_arrivalTrianglePtr.GetTriangleIdx() != NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(halfEdgeIdRawPtrHit.GetHalfEdgeIdx()))
		{
			if (KY_FAILED(crossedTriangles.PushBack(m_arrivalTrianglePtr.GetRawPtr())))
			{
				KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
				SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
				return;
			}
		}
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
	{
		navTagSubSegment.m_exitPos3f = m_arrivalPos3f;
		if(KY_FAILED(navTagSubSegments.PushBack(navTagSubSegment)))
		{
			KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
			SetResult(RAYCAST_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}
	}

	SetResult(RAYCAST_DONE_ARRIVALPOS_FOUND_AFTER_COLLISION);
}
}
