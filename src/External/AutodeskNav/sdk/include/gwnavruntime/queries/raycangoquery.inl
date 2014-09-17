/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{
template<class NavTagTraversePredicate>
KY_INLINE RayCanGoQuery<NavTagTraversePredicate>::RayCanGoQuery() : BaseRayCanGoQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE RayCanGoQuery<NavTagTraversePredicate>::RayCanGoQuery(const NavTagTraversePredicate& predicate) : BaseRayCanGoQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos)
{
	BaseRayCanGoQuery::Initialize(database, startPos, destPos);
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr) { BaseRayCanGoQuery::SetStartTrianglePtr(startTrianglePtr);   }
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)  { BaseRayCanGoQuery::SetStartIntegerPos(startIntegerPos);     }
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetDestIntegerPos(const WorldIntegerPos& destIntegerPos)    { BaseRayCanGoQuery::SetDestIntegerPos(destIntegerPos);       }
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)          { BaseRayCanGoQuery::SetDynamicOutputMode(savingMode);        }
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)
{
	BaseRayCanGoQuery::SetQueryDynamicOutput(queryDynamicOutput);
}
template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseRayCanGoQuery::SetPositionSpatializationRange(positionSpatializationRange);
}

template<class NavTagTraversePredicate>
KY_INLINE RayCanGoQueryResult   RayCanGoQuery<NavTagTraversePredicate>::GetResult()                 const { return BaseRayCanGoQuery::GetResult();                 }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode     RayCanGoQuery<NavTagTraversePredicate>::GetDynamicOutputMode()      const { return BaseRayCanGoQuery::GetDynamicOutputMode();      }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&          RayCanGoQuery<NavTagTraversePredicate>::GetStartPos()               const { return BaseRayCanGoQuery::GetStartPos();               }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&          RayCanGoQuery<NavTagTraversePredicate>::GetDestPos()                const { return BaseRayCanGoQuery::GetDestPos();                }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr& RayCanGoQuery<NavTagTraversePredicate>::GetStartTrianglePtr()       const { return BaseRayCanGoQuery::GetStartTrianglePtr();       }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr& RayCanGoQuery<NavTagTraversePredicate>::GetDestTrianglePtr()        const { return BaseRayCanGoQuery::GetDestTrianglePtr();        }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*   RayCanGoQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()     const { return BaseRayCanGoQuery::GetQueryDynamicOutput();     }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& RayCanGoQuery<NavTagTraversePredicate>::GetStartIntegerPos()       const { return BaseRayCanGoQuery::GetStartIntegerPos();        }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& RayCanGoQuery<NavTagTraversePredicate>::GetDestIntegerPos()        const { return BaseRayCanGoQuery::GetDestIntegerPos();         }

template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& RayCanGoQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const
{
	return BaseRayCanGoQuery::GetPositionSpatializationRange();
}

template<class NavTagTraversePredicate>
KY_INLINE void RayCanGoQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
inline void RayCanGoQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != RAYCANGO_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(RAYCANGO_DONE_START_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_startPos3f, m_startIntegerPos);
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_destPos3f, m_destIntegerPos);

	PerformQueryWithInputCoordPos(workingMemory);
}


template<class NavTagTraversePredicate>
inline void RayCanGoQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == RAYCANGO_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	KyFloat32 integerPrecision = databaseGenMetrics.m_integerPrecision;

	WorkingMemArray<NavTriangleRawPtr> crossedTriangles;
	WorkingMemArray<RawNavTagSubSegment> navTagSubSegments;
	WorkingMemArray<RawHalfEdgeIntersection> halfEdgeIntersections;

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_startIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(RAYCANGO_DONE_START_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr startTriangleRawPtr = m_startTrianglePtr.GetRawPtr();

	if (m_predicate.CanTraverse(startTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(RAYCANGO_DONE_START_NAVTAG_FORBIDDEN);
		return;
	}

	ScopeAutoSaveDynamicOutput scopeAutoSaveDynOutput(m_queryDynamicOutput);
	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		crossedTriangles.Init(workingMemory);
		if (crossedTriangles.IsInitialized() == false)
		{
			SetResult(RAYCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&crossedTriangles);

		if (KY_FAILED(crossedTriangles.PushBack(startTriangleRawPtr)))
		{
			KY_LOG_WARNING( ("This query reached the maximum size of working memory"));
			SetResult(RAYCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS) != 0)
	{
		navTagSubSegments.Init(workingMemory);
		if (navTagSubSegments.IsInitialized() == false)
		{
			SetResult(RAYCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetRawNavTagSubSegments(&navTagSubSegments);
	}

	if ((GetDynamicOutputMode() & QUERY_SAVE_HALFEDGEINTERSECTIONS) != 0)
	{
		halfEdgeIntersections.Init(workingMemory);
		if (halfEdgeIntersections.IsInitialized() == false)
		{
			SetResult(RAYCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetRawHalfEdgeIntersections(&halfEdgeIntersections);
	}

	RayQueryUtils rayQueryUtils(m_startPos3f, m_destPos3f, GetDynamicOutputMode(), &crossedTriangles, &navTagSubSegments, &halfEdgeIntersections);
	const bool storePropagationData =
		(GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES            ) != 0 ||
		(GetDynamicOutputMode() & QUERY_SAVE_NAVTAGSUBSEGMENTS     ) != 0 ||
		(GetDynamicOutputMode() & QUERY_SAVE_HALFEDGEINTERSECTIONS) != 0;

	// not is same pos, use CoordPos64
#if defined (KY_BUILD_DEBUG)
	const CellPos minCellPos(Min(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x), Min(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y));
	const CellPos maxCellPos(Max(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x) + 1, Max(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y) + 1);
	const CoordBox64 cellsIntegerBox(databaseGenMetrics.ComputeCellOrigin(minCellPos), databaseGenMetrics.ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 31))
		KY_LOG_WARNING( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
#endif

	RawNavTagSubSegment currentNavTagSubSegment;
	currentNavTagSubSegment.m_navTriangleRawPtr = startTriangleRawPtr;
	currentNavTagSubSegment.m_entrancePos3f = m_startPos3f;

	const CoordPos64 startCoordPos = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_startIntegerPos);
	const CoordPos64 destCoordPos = databaseGenMetrics.GetCoordPos64FromWorldIntegerPos(m_destIntegerPos);

	const CoordPos64 direction(destCoordPos - startCoordPos); // 31 bits
	const KyInt64 startToDestSquareDist = DotProduct(direction, direction);

	KyInt32 edgeNumberToCrossInTriangle = rayQueryUtils.GetfirstEdgeToCrossInStartTriangle(startTriangleRawPtr, startCoordPos, destCoordPos);
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
		startCoordPos, direction, destCoordPos, startToDestSquareDist) == false)
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
					SetResult(RAYCANGO_DONE_LACK_OF_WORKING_MEMORY);
					return;
				}
			}

			// before looping, we compute which edge we will cross in the triangle
			// for that, we look on which side of the direction the third vertex of the triangle is.
			const NavVertex& thirdVertexPosInFloor = currentNavFloorBlob->NavHalfEdgeIdxToThirdNavVertex(currentPairHalfEdgeRawPtr.GetHalfEdgeIdx());
			const CoordPos64 thirdVertexPos = currentCellOrigin + thirdVertexPosInFloor.GetCoordPos64();
			const KyInt64 crossProductToChooseTheGoodEdge = CrossProduct(thirdVertexPos - startCoordPos, direction);

			if (crossProductToChooseTheGoodEdge > 0)
				// go on the left of third
				currentPairHalfEdgeRawPtr.GetPrevHalfEdgeRawPtr(currentHalfEdgeRawPtrToCross);
			else
				// go on the right of third
				currentPairHalfEdgeRawPtr.GetNextHalfEdgeRawPtr(currentHalfEdgeRawPtrToCross);

			currentNavHalfEdge = currentNavFloorBlob->GetNavHalfEdge(currentHalfEdgeRawPtrToCross.GetHalfEdgeIdx());
			continue;
		}

		SetResult(RAYCANGO_DONE_COLLISION_DETECTED);
		return;
	}

	RayCanGo_NoHit(currentHalfEdgeRawPtrToCross, currentNavTagSubSegment, navTagSubSegments);
}

}
