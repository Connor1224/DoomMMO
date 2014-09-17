/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

template<class NavTagTraversePredicate>
KY_INLINE DiskCollisionQuery<NavTagTraversePredicate>::DiskCollisionQuery() : BaseDiskCollisionQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE DiskCollisionQuery<NavTagTraversePredicate>::DiskCollisionQuery(const NavTagTraversePredicate& predicate) : BaseDiskCollisionQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& center, KyFloat32 radius)
{
	BaseDiskCollisionQuery::Initialize(database, center, radius);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::SetCenterTrianglePtr(const NavTrianglePtr& centerTrianglePtr)
{
	BaseDiskCollisionQuery::SetCenterTrianglePtr(centerTrianglePtr);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::SetCenterIntegerPos(const WorldIntegerPos& centerIntegerPos)
{
	BaseDiskCollisionQuery::SetCenterIntegerPos(centerIntegerPos);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseDiskCollisionQuery::SetPositionSpatializationRange(positionSpatializationRange);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode dynamicOutputMode)
{
	BaseDiskCollisionQuery::SetDynamicOutputMode(dynamicOutputMode);
}
template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)
{
	BaseDiskCollisionQuery::SetQueryDynamicOutput(queryDynamicOutput);
}

template<class NavTagTraversePredicate>
KY_INLINE DiskCollisionQueryResult DiskCollisionQuery<NavTagTraversePredicate>::GetResult()                 const { return BaseDiskCollisionQuery::GetResult();                 }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode        DiskCollisionQuery<NavTagTraversePredicate>::GetDynamicOutputMode()      const { return BaseDiskCollisionQuery::GetDynamicOutputMode();      }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&             DiskCollisionQuery<NavTagTraversePredicate>::GetCenterPos()              const { return BaseDiskCollisionQuery::GetCenterPos();              }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&    DiskCollisionQuery<NavTagTraversePredicate>::GetCenterTrianglePtr()      const { return BaseDiskCollisionQuery::GetCenterTrianglePtr();      }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32                DiskCollisionQuery<NavTagTraversePredicate>::GetRadius()                 const { return BaseDiskCollisionQuery::GetRadius();                 }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*      DiskCollisionQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()     const { return BaseDiskCollisionQuery::GetQueryDynamicOutput();     }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&   DiskCollisionQuery<NavTagTraversePredicate>::GetCenterIntegerPos()       const { return BaseDiskCollisionQuery::GetCenterIntegerPos();       }
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& DiskCollisionQuery<NavTagTraversePredicate>::GetPositionSpatializationRange() const { return BaseDiskCollisionQuery::GetPositionSpatializationRange(); }

template<class NavTagTraversePredicate>
KY_INLINE void DiskCollisionQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
void DiskCollisionQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != DISKCOLLISION_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(DISKCOLLISION_DONE_CENTER_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_centerPos3f, m_centerIntegerPos);

	return PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
void DiskCollisionQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == DISKCOLLISION_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	TraversalResult traversalRC;
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_centerTrianglePtr, m_centerPos3f, m_centerIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(DISKCOLLISION_DONE_CENTER_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr centerTriangleRawPtr = m_centerTrianglePtr.GetRawPtr();

	if(m_predicate.CanTraverse(centerTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(DISKCOLLISION_DONE_CENTER_NAVTAG_FORBIDDEN);
		return;
	}

	const KyInt32 radiusCellSize = (KyInt32)(databaseGenMetrics.GetNearestInteger64FromFloatValue(m_radius) / databaseGenMetrics.m_cellSizeInCoord) + 1;
	const CellPos minCellPos(m_centerIntegerPos.m_cellPos.x - radiusCellSize, m_centerIntegerPos.m_cellPos.y - radiusCellSize);
	const CellPos maxCellPos(m_centerIntegerPos.m_cellPos.x + radiusCellSize, m_centerIntegerPos.m_cellPos.y + radiusCellSize);
	const CellBox cellBox(minCellPos, maxCellPos);

#if defined (KY_BUILD_DEBUG)
	const CoordBox64 cellsIntegerBox(databaseGenMetrics.ComputeCellOrigin(minCellPos), databaseGenMetrics.ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 31))
		KY_DEBUG_WARNINGN( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
#endif

	// Set up the visitor and the traversal
	DiskIntersector diskIntersector(*this);
	BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, DiskIntersector> edgeIntersectionVisitor(m_predicate, diskIntersector);
	BreadthFirstSearchTraversal<BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, DiskIntersector> > traversalDiskCollision(queryUtils, cellBox,
		edgeIntersectionVisitor);

	// set the root node : the triangle that contains m_startPos
	// Not that its navTag is not test within this function, but it has already been tested
	traversalRC = traversalDiskCollision.SetStartTriangle(centerTriangleRawPtr);
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCollisionResult(traversalRC));
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
			SetResult(DISKCOLLISION_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&visitedTriangles);

		traversalDiskCollision.SetVisitedNodeContainer(&visitedTriangles);
	}

	// run the traversal algorithm
	traversalRC = traversalDiskCollision.Search();
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCollisionResult(traversalRC));
		return;
	}

	// retrieve the result of the traversal
	SetResult(edgeIntersectionVisitor.m_collisionFound ? DISKCOLLISION_DONE_DISK_DOES_NOT_FIT : DISKCOLLISION_DONE_DISK_FIT);
}


}
