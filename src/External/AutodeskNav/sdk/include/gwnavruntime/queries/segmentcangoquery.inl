/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


namespace Kaim
{

// Primary contact: JUBA - secondary contact: NOBODY

template<class NavTagTraversePredicate>
KY_INLINE SegmentCanGoQuery<NavTagTraversePredicate>::SegmentCanGoQuery() : BaseSegmentCanGoQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE SegmentCanGoQuery<NavTagTraversePredicate>::SegmentCanGoQuery(const NavTagTraversePredicate& predicate) : BaseSegmentCanGoQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos, KyFloat32 radius)
{
	BaseSegmentCanGoQuery::Initialize(database, startPos, destPos, radius);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr)
{
	BaseSegmentCanGoQuery::SetStartTrianglePtr(startTrianglePtr);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseSegmentCanGoQuery::SetPositionSpatializationRange(positionSpatializationRange);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)
{
	BaseSegmentCanGoQuery::SetStartIntegerPos(startIntegerPos);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetDestIntegerPos(const WorldIntegerPos& destIntegerPos)
{
	BaseSegmentCanGoQuery::SetDestIntegerPos(destIntegerPos);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)
{
	BaseSegmentCanGoQuery::SetDynamicOutputMode(savingMode);
}
template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)
{
	BaseSegmentCanGoQuery::SetQueryDynamicOutput(queryDynamicOutput);
}

template<class NavTagTraversePredicate>
KY_INLINE SegmentCanGoQueryResult SegmentCanGoQuery<NavTagTraversePredicate>::GetResult()                  const { return BaseSegmentCanGoQuery::GetResult();                 }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode       SegmentCanGoQuery<NavTagTraversePredicate>::GetDynamicOutputMode()       const { return BaseSegmentCanGoQuery::GetDynamicOutputMode();      }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&            SegmentCanGoQuery<NavTagTraversePredicate>::GetStartPos()                const { return BaseSegmentCanGoQuery::GetStartPos();               }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&            SegmentCanGoQuery<NavTagTraversePredicate>::GetDestPos()                 const { return BaseSegmentCanGoQuery::GetDestPos();                }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&   SegmentCanGoQuery<NavTagTraversePredicate>::GetStartTrianglePtr()        const { return BaseSegmentCanGoQuery::GetStartTrianglePtr();       }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&   SegmentCanGoQuery<NavTagTraversePredicate>::GetDestTrianglePtr()         const { return BaseSegmentCanGoQuery::GetDestTrianglePtr();        }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32               SegmentCanGoQuery<NavTagTraversePredicate>::GetRadius()                  const { return BaseSegmentCanGoQuery::GetRadius();                 }
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& SegmentCanGoQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const
{
	return BaseSegmentCanGoQuery::GetPositionSpatializationRange();
}
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*     SegmentCanGoQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()      const { return BaseSegmentCanGoQuery::GetQueryDynamicOutput();     }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&  SegmentCanGoQuery<NavTagTraversePredicate>::GetStartIntegerPos()         const { return BaseSegmentCanGoQuery::GetStartIntegerPos();        }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&  SegmentCanGoQuery<NavTagTraversePredicate>::GetDestIntegerPos()          const { return BaseSegmentCanGoQuery::GetDestIntegerPos();         }

template<class NavTagTraversePredicate>
KY_INLINE void SegmentCanGoQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
inline void SegmentCanGoQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != SEGMENTCANGO_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(SEGMENTCANGO_DONE_START_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_startPos3f, m_startIntegerPos);
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_destPos3f, m_destIntegerPos);

	return PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
inline void SegmentCanGoQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == SEGMENTCANGO_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	TraversalResult traversalRC;

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_startIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(SEGMENTCANGO_DONE_START_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr startTriangleRawPtr = m_startTrianglePtr.GetRawPtr();

	if(m_predicate.CanTraverse(startTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(SEGMENTCANGO_DONE_START_NAVTAG_FORBIDDEN);
		return;
	}

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	const KyInt32 radiusCellSize = (KyInt32)(databaseGenMetrics.GetNearestInteger64FromFloatValue(m_radius) / m_database->GetDatabaseGenMetrics().m_cellSizeInCoord) + 1;
	const CellPos minCellPos(Min(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x) - radiusCellSize,
	                         Min(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y) - radiusCellSize);
	const CellPos maxCellPos(Max(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x) + radiusCellSize,
	                         Max(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y) + radiusCellSize);
	const CellBox cellBox(minCellPos, maxCellPos);


#if defined (KY_BUILD_DEBUG)
	const CoordBox64 cellsIntegerBox(m_database->GetDatabaseGenMetrics().ComputeCellOrigin(minCellPos), m_database->GetDatabaseGenMetrics().ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 31))
		KY_LOG_WARNING( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
#endif

	// Set up the visitor and the traversal
	RectangleIntersector rectangleIntersector(*this);
	BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, RectangleIntersector> edgeIntersectionVisitor(m_predicate, rectangleIntersector);
	BreadthFirstSearchTraversal<BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, RectangleIntersector> > traversalSegmentCanGo(queryUtils, cellBox,
		edgeIntersectionVisitor);

	// set the root node : the triangle that contains m_startPos
	// Not that its navTag is not test within this function, but it has already been tested
	traversalRC = traversalSegmentCanGo.SetStartTriangle(startTriangleRawPtr);
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToSegmentCanGoResult(traversalRC));
		return;
	}

	// check if we want to retrieve the visited triangles
	WorkingMemArray<NavTriangleRawPtr> visitedTriangles;
	ScopeAutoSaveDynamicOutput scopeAutoSaveDynOutput(m_queryDynamicOutput);
	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		visitedTriangles.Init(workingMemory);
		if (visitedTriangles.IsInitialized() == false)
		{
			SetResult(SEGMENTCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&visitedTriangles);
		traversalSegmentCanGo.SetVisitedNodeContainer(&visitedTriangles);
	}

	// run the traversal algorithm
	traversalRC = traversalSegmentCanGo.Search();
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToSegmentCanGoResult(traversalRC));
		return;
	}

	// retrieve the result of the traversal
	if (edgeIntersectionVisitor.m_collisionFound)
	{
		SetResult(SEGMENTCANGO_DONE_COLLISION_DETECTED);
		return;
	}

	// check the floor with a RayCanGo
	RayCanGoQuery<NavTagTraversePredicate> rayCanGoQuery(m_predicate);
	rayCanGoQuery.Initialize(m_database, m_startPos3f, m_destPos3f);
	rayCanGoQuery.SetStartTrianglePtr(m_startTrianglePtr);
	rayCanGoQuery.SetStartIntegerPos(m_startIntegerPos);
	rayCanGoQuery.SetDestIntegerPos(m_destIntegerPos);
	rayCanGoQuery.SetPositionSpatializationRange(m_positionSpatializationRange);

	rayCanGoQuery.PerformQueryWithInputCoordPos(workingMemory);

	m_destTrianglePtr = rayCanGoQuery.GetDestTrianglePtr();
	SetResult(ConvertRayCanGoResultToSegmentCanGoResult(rayCanGoQuery.GetResult()));
}


}

