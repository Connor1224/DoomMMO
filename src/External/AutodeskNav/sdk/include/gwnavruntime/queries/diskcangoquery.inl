/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY

namespace Kaim
{
template<class NavTagTraversePredicate>
KY_INLINE DiskCanGoQuery<NavTagTraversePredicate>::DiskCanGoQuery() : BaseDiskCanGoQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE DiskCanGoQuery<NavTagTraversePredicate>::DiskCanGoQuery(const NavTagTraversePredicate& predicate) : BaseDiskCanGoQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos, KyFloat32 radius)
{
	BaseDiskCanGoQuery::Initialize(database, startPos, destPos, radius);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr)   { BaseDiskCanGoQuery::SetStartTrianglePtr(startTrianglePtr);     }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)    { BaseDiskCanGoQuery::SetStartIntegerPos(startIntegerPos);       }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetDestIntegerPos(const WorldIntegerPos& destIntegerPos)      { BaseDiskCanGoQuery::SetDestIntegerPos(destIntegerPos);         }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetQueryType(DiskCanGoQueryType queryType)                    { BaseDiskCanGoQuery::SetQueryType(queryType);                   }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)            { BaseDiskCanGoQuery::SetDynamicOutputMode(savingMode);          }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput) { BaseDiskCanGoQuery::SetQueryDynamicOutput(queryDynamicOutput); }
template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseDiskCanGoQuery::SetPositionSpatializationRange(positionSpatializationRange);
}

template<class NavTagTraversePredicate>
KY_INLINE DiskCanGoQueryResult   DiskCanGoQuery<NavTagTraversePredicate>::GetResult()                  const { return BaseDiskCanGoQuery::GetResult();                  }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode      DiskCanGoQuery<NavTagTraversePredicate>::GetDynamicOutputMode()       const { return BaseDiskCanGoQuery::GetDynamicOutputMode();       }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&           DiskCanGoQuery<NavTagTraversePredicate>::GetStartPos()                const { return BaseDiskCanGoQuery::GetStartPos();                }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&           DiskCanGoQuery<NavTagTraversePredicate>::GetDestPos()                 const { return BaseDiskCanGoQuery::GetDestPos();                 }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&  DiskCanGoQuery<NavTagTraversePredicate>::GetStartTrianglePtr()        const { return BaseDiskCanGoQuery::GetStartTrianglePtr();        }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&  DiskCanGoQuery<NavTagTraversePredicate>::GetDestTrianglePtr()         const { return BaseDiskCanGoQuery::GetDestTrianglePtr();         }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32              DiskCanGoQuery<NavTagTraversePredicate>::GetRadius()                  const { return BaseDiskCanGoQuery::GetRadius();                  }
template<class NavTagTraversePredicate>
KY_INLINE DiskCanGoQueryType     DiskCanGoQuery<NavTagTraversePredicate>::GetQueryType()               const { return BaseDiskCanGoQuery::GetQueryType();               }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*    DiskCanGoQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()      const { return BaseDiskCanGoQuery::GetQueryDynamicOutput();      }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& DiskCanGoQuery<NavTagTraversePredicate>::GetStartIntegerPos()         const { return BaseDiskCanGoQuery::GetStartIntegerPos();         }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos& DiskCanGoQuery<NavTagTraversePredicate>::GetDestIntegerPos()          const { return BaseDiskCanGoQuery::GetDestIntegerPos();          }
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& DiskCanGoQuery<NavTagTraversePredicate>::GetPositionSpatializationRange()  const { return BaseDiskCanGoQuery::GetPositionSpatializationRange();  }

template<class NavTagTraversePredicate>
KY_INLINE void DiskCanGoQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
inline void DiskCanGoQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != DISKCANGO_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(DISKCANGO_DONE_START_OUTSIDE);
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
inline void DiskCanGoQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == DISKCANGO_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	TraversalResult traversalRC;

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_startTrianglePtr, m_startPos3f, m_startIntegerPos, m_positionSpatializationRange) == false)
	{
		SetResult(DISKCANGO_DONE_START_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr startTriangleRawPtr = m_startTrianglePtr.GetRawPtr();

	if(m_predicate.CanTraverse(startTriangleRawPtr.GetNavTag()) == false)
	{
		SetResult(DISKCANGO_DONE_START_NAVTAG_FORBIDDEN);
		return;
	}

	const KyInt32 radiusCellSize = (KyInt32)(databaseGenMetrics.GetNearestInteger64FromFloatValue(m_radius) / databaseGenMetrics.m_cellSizeInCoord) + 1;
	const CellPos minCellPos(Min(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x) - radiusCellSize,
	                         Min(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y) - radiusCellSize);
	const CellPos maxCellPos(Max(m_startIntegerPos.m_cellPos.x, m_destIntegerPos.m_cellPos.x) + radiusCellSize,
	                         Max(m_startIntegerPos.m_cellPos.y, m_destIntegerPos.m_cellPos.y) + radiusCellSize);
	const CellBox cellBox(minCellPos, maxCellPos);

#if defined (KY_BUILD_DEBUG)
	const CoordBox64 cellsIntegerBox(databaseGenMetrics.ComputeCellOrigin(minCellPos), databaseGenMetrics.ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 31))
		KY_DEBUG_WARNINGN( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
#endif

	if (m_queryType == DISKCANGO_ADAPT_TO_STARTPOS)
	{
		DiskCollisionQuery<NavTagTraversePredicate> diskCollisionQuery(m_predicate);
		diskCollisionQuery.Initialize(m_database, m_startPos3f, m_radius);
		diskCollisionQuery.SetCenterTrianglePtr(m_startTrianglePtr);
		diskCollisionQuery.SetCenterIntegerPos(m_startIntegerPos);
		diskCollisionQuery.SetPositionSpatializationRange(m_positionSpatializationRange);

		diskCollisionQuery.PerformQueryWithInputCoordPos(workingMemory);

		if (diskCollisionQuery.GetResult() != DISKCOLLISION_DONE_DISK_FIT)
		{
			switch (diskCollisionQuery.GetResult())
			{
			case DISKCOLLISION_DONE_DISK_DOES_NOT_FIT :
				ForceDiskCanGo(workingMemory, cellBox, startTriangleRawPtr);
				break;

			case DISKCOLLISION_DONE_CENTER_NAVTAG_FORBIDDEN :
				SetResult(DISKCANGO_DONE_START_NAVTAG_FORBIDDEN);
				break;

			case DISKCOLLISION_DONE_LACK_OF_WORKING_MEMORY :
				SetResult(DISKCANGO_DONE_LACK_OF_WORKING_MEMORY);
				break;

			default :
				SetResult(DISKCANGO_DONE_UNKNOWN_ERROR);
			}

			return;
		}
	}

	// Set up the visitor and the traversal
	CapsuleIntersector capsuleIntersector(*this);
	BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, CapsuleIntersector> edgeIntersectionVisitor(m_predicate, capsuleIntersector);
	BreadthFirstSearchTraversal<BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, CapsuleIntersector> > traversalDiskCanGo(queryUtils, cellBox,
		edgeIntersectionVisitor);
	
	// set the root node : the triangle that contains m_startPos
	// Not that its navTag is not test within this function, but it has already been tested
	traversalRC = traversalDiskCanGo.SetStartTriangle(startTriangleRawPtr);
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCanGoResult(traversalRC));
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
			SetResult(DISKCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&visitedTriangles);
		traversalDiskCanGo.SetVisitedNodeContainer(&visitedTriangles);
	}

	// run the traversal algorithm
	traversalRC = traversalDiskCanGo.Search();
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCanGoResult(traversalRC));
		return;
	}

	// retrieve the result of the traversal
	if (edgeIntersectionVisitor.m_collisionFound)
	{
		SetResult(DISKCANGO_DONE_COLLISION_DETECTED);
		return;
	}

	// No collision during the propagation, now we have to check if we arrive in the good floor
	// according to m_destPos3f
	// we check that with a RayCanGo taht checks the altitude difference between m_destPos3f and its
	// projection in the arrival Triangle
	RayCanGoQuery<NavTagTraversePredicate> rayCanGoQuery(m_predicate);
	rayCanGoQuery.Initialize(m_database, m_startPos3f, m_destPos3f);
	rayCanGoQuery.SetStartTrianglePtr(m_startTrianglePtr);
	rayCanGoQuery.SetStartIntegerPos(m_startIntegerPos);
	rayCanGoQuery.SetDestIntegerPos(m_destIntegerPos);
	rayCanGoQuery.SetPositionSpatializationRange(m_positionSpatializationRange);

	rayCanGoQuery.PerformQueryWithInputCoordPos(workingMemory);

	m_destTrianglePtr = rayCanGoQuery.GetDestTrianglePtr();
	SetResult(ConvertRayCanGoResultToDiskCanGoResult(rayCanGoQuery.GetResult()));
}

template<class NavTagTraversePredicate>
inline void DiskCanGoQuery<NavTagTraversePredicate>::ForceDiskCanGo(WorkingMemory* workingMemory, const CellBox& cellbox, const NavTriangleRawPtr& startTriangleRawPtr)
{
	QueryUtils queryUtils(m_database, workingMemory);
	TraversalResult traversalRC;
	KY_LOG_ERROR_IF(m_startTrianglePtr.IsValid() == false, ("m_startTrianglePtr must have been computed here. It must be valid !"));

	Vec2f normalizedDir2d = Vec2f(m_destPos3f.x - m_startPos3f.x, m_destPos3f.y - m_startPos3f.y);
	normalizedDir2d.Normalize();

	RayCastQueryResult queryOnRightResult, queryOnLeftResult;
	KyFloat32 borderDistOnRight = 0.f;
	KyFloat32 borderDistOnLeft = 0.f;
	RayCastQuery<NavTagTraversePredicate> rayCastQuery(m_predicate);

	if (KY_FAILED(RayCastQueryFromStartOnTheRigth(workingMemory, normalizedDir2d, rayCastQuery, queryOnRightResult, borderDistOnRight)))
		return;

	if (KY_FAILED(RayCastQueryFromStartOnTheLeft(workingMemory, normalizedDir2d, rayCastQuery, queryOnLeftResult, borderDistOnLeft)))
		return;

	if (queryOnLeftResult == RAYCAST_DONE_CANNOT_MOVE && queryOnRightResult == RAYCAST_DONE_CANNOT_MOVE)
	{
		SetResult(DISKCANGO_DONE_COLLISION_DETECTED);
		return;
	}

	// Set up the visitor and the traversal
	CrossedSectionCapsuleIntersector crossedSectionCapsuleIntersector(*this, borderDistOnRight, borderDistOnLeft);
	BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, CrossedSectionCapsuleIntersector> edgeIntersectionVisitor(m_predicate, crossedSectionCapsuleIntersector);
	BreadthFirstSearchTraversal<BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, CrossedSectionCapsuleIntersector> > traversalDiskCanGo(queryUtils, cellbox,
		edgeIntersectionVisitor);

	// set the root node : the triangle that contains m_startPos
	traversalRC = traversalDiskCanGo.SetStartTriangle(startTriangleRawPtr);
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCanGoResult(traversalRC));
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
			SetResult(DISKCANGO_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&visitedTriangles);
		traversalDiskCanGo.SetVisitedNodeContainer(&visitedTriangles);
	}

	// run the traversal algorithm
	traversalRC = traversalDiskCanGo.Search();
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskCanGoResult(traversalRC));
		return;
	}

	// retrieve the result of the traversal
	if (edgeIntersectionVisitor.m_collisionFound)
	{
		SetResult(DISKCANGO_DONE_COLLISION_DETECTED);
		return;
	}

	// No collision during the propagation, now we have to check if we arrive in the good floor
	// according to m_destPos3f
	// we check that with a RayCanGo taht checks the altitude difference between m_destPos3f and its
	// projection in the arrival Triangle
	RayCanGoQuery<NavTagTraversePredicate> rayCanGoQuery(m_predicate);
	rayCanGoQuery.Initialize(m_database, m_startPos3f, m_destPos3f);
	rayCanGoQuery.SetStartTrianglePtr(m_startTrianglePtr);
	rayCanGoQuery.SetStartIntegerPos(m_startIntegerPos);
	rayCanGoQuery.SetDestIntegerPos(m_destIntegerPos);
	rayCanGoQuery.SetPositionSpatializationRange(m_positionSpatializationRange);

	rayCanGoQuery.PerformQueryWithInputCoordPos(workingMemory);

	m_destTrianglePtr = rayCanGoQuery.GetDestTrianglePtr();
	SetResult(ConvertRayCanGoResultToDiskCanGoResult(rayCanGoQuery.GetResult()));
}

template<class NavTagTraversePredicate>
inline KyResult DiskCanGoQuery<NavTagTraversePredicate>::RayCastQueryFromStartOnTheRigth(WorkingMemory* workingMemory, const Vec2f& normalizedDir2d,
	RayCastQuery<NavTagTraversePredicate>& rayCastQuery, RayCastQueryResult& queryOnRightResult, KyFloat32& borderDistOnRight)
{
	RayCastQuery<NavTagTraversePredicate>& queryOnRight = rayCastQuery;
	queryOnRight.Initialize(m_database, m_startPos3f, normalizedDir2d.PerpCW(), m_radius);
	queryOnRight.SetStartTrianglePtr(m_startTrianglePtr);
	queryOnRight.SetStartIntegerPos(m_startIntegerPos);
	queryOnRight.SetPositionSpatializationRange(m_positionSpatializationRange);

	queryOnRight.PerformQueryWithInputCoordPos(workingMemory);

	queryOnRightResult = queryOnRight.GetResult();
	if (queryOnRightResult == RAYCAST_DONE_COLLISION_FOUND_ARRIVAL_ERROR)
	{
		SetResult(DISKCANGO_DONE_COLLISION_DETECTED);
		return KY_ERROR;
	}

	if(queryOnRight.GetResult() == RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED)
	{
		borderDistOnRight = m_radius;
	}
	else
	{
		if (queryOnRightResult == RAYCAST_DONE_CANNOT_MOVE)
			borderDistOnRight = 0.f;
		else
			borderDistOnRight = (queryOnRight.GetArrivalPos() - queryOnRight.GetStartPos()).GetLength2d();
	}

	return KY_SUCCESS;
}


template<class NavTagTraversePredicate>
inline KyResult DiskCanGoQuery<NavTagTraversePredicate>::RayCastQueryFromStartOnTheLeft(WorkingMemory* workingMemory, const Vec2f& normalizedDir2d,
	RayCastQuery<NavTagTraversePredicate>& rayCastQuery, RayCastQueryResult& queryOnLeftResult, KyFloat32& borderDistOnLeft)
{
	RayCastQuery<NavTagTraversePredicate>& queryOnLeft = rayCastQuery;
	queryOnLeft.Initialize(m_database, m_startPos3f, normalizedDir2d.PerpCCW(), m_radius);
	queryOnLeft.SetStartTrianglePtr(m_startTrianglePtr);
	queryOnLeft.SetStartIntegerPos(m_startIntegerPos);
	queryOnLeft.SetPositionSpatializationRange(m_positionSpatializationRange);

	queryOnLeft.PerformQueryWithInputCoordPos(workingMemory);

	queryOnLeftResult = queryOnLeft.GetResult();
	if (queryOnLeftResult == RAYCAST_DONE_COLLISION_FOUND_ARRIVAL_ERROR)
	{
		SetResult(DISKCANGO_DONE_COLLISION_DETECTED);
		return KY_ERROR;
	}

	if (queryOnLeft.GetResult() == RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED)
		borderDistOnLeft = m_radius;
	else
	{
		if(queryOnLeftResult == RAYCAST_DONE_CANNOT_MOVE)
			borderDistOnLeft = 0.f;
		else
			borderDistOnLeft = (queryOnLeft.GetArrivalPos() - queryOnLeft.GetStartPos()).GetLength2d();
	}

	return KY_SUCCESS;
}

}
