/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

template<class NavTagTraversePredicate>
KY_INLINE DiskExpansionQuery<NavTagTraversePredicate>::DiskExpansionQuery() : BaseDiskExpansionQuery(), m_predicate() {}
template<class NavTagTraversePredicate>
KY_INLINE DiskExpansionQuery<NavTagTraversePredicate>::DiskExpansionQuery(const NavTagTraversePredicate& predicate) : BaseDiskExpansionQuery(), m_predicate(predicate) {}

template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::Initialize(Database* database, const Vec3f& center, KyFloat32 radiusMax)
{
	BaseDiskExpansionQuery::Initialize(database, center, radiusMax);
}

template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetCenterTrianglePtr(const NavTrianglePtr& centerTrianglePtr) { BaseDiskExpansionQuery::SetCenterTrianglePtr(centerTrianglePtr);   }
template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetCenterIntegerPos(const WorldIntegerPos& centerIntegerPos)  { BaseDiskExpansionQuery::SetCenterIntegerPos(centerIntegerPos);     }
template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetSafetyDist(KyFloat32 safetyDist)                           { BaseDiskExpansionQuery::SetSafetyDist(safetyDist);                 }
template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetDynamicOutputMode(DynamicOutputMode savingMode)            { BaseDiskExpansionQuery::SetDynamicOutputMode(savingMode);          }
template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput) { BaseDiskExpansionQuery::SetQueryDynamicOutput(queryDynamicOutput); }

template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	BaseDiskExpansionQuery::SetPositionSpatializationRange(positionSpatializationRange);
}

template<class NavTagTraversePredicate>
KY_INLINE DiskExpansionQueryResult DiskExpansionQuery<NavTagTraversePredicate>::GetResult()                  const { return BaseDiskExpansionQuery::GetResult();                  }
template<class NavTagTraversePredicate>
KY_INLINE DynamicOutputMode        DiskExpansionQuery<NavTagTraversePredicate>::GetDynamicOutputMode()       const { return BaseDiskExpansionQuery::GetDynamicOutputMode();       }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&             DiskExpansionQuery<NavTagTraversePredicate>::GetCenterPos()               const { return BaseDiskExpansionQuery::GetCenterPos();               }
template<class NavTagTraversePredicate>
KY_INLINE const NavTrianglePtr&    DiskExpansionQuery<NavTagTraversePredicate>::GetCenterTrianglePtr()       const { return BaseDiskExpansionQuery::GetCenterTrianglePtr();       }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32                DiskExpansionQuery<NavTagTraversePredicate>::GetRadiusMax()               const { return BaseDiskExpansionQuery::GetRadiusMax();               }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32                DiskExpansionQuery<NavTagTraversePredicate>::GetResultRadius()            const { return BaseDiskExpansionQuery::GetResultRadius();            }
template<class NavTagTraversePredicate>
KY_INLINE const Vec3f&             DiskExpansionQuery<NavTagTraversePredicate>::GetCollisionPos()            const { return BaseDiskExpansionQuery::GetCollisionPos();            }
template<class NavTagTraversePredicate>
KY_INLINE const NavHalfEdgePtr&    DiskExpansionQuery<NavTagTraversePredicate>::GetCollisionNavHalfEdgePtr() const { return BaseDiskExpansionQuery::GetCollisionNavHalfEdgePtr(); }
template<class NavTagTraversePredicate>
KY_INLINE KyFloat32                DiskExpansionQuery<NavTagTraversePredicate>::GetSafetyDist()              const { return BaseDiskExpansionQuery::GetSafetyDist();              }
template<class NavTagTraversePredicate>
KY_INLINE QueryDynamicOutput*      DiskExpansionQuery<NavTagTraversePredicate>::GetQueryDynamicOutput()      const { return BaseDiskExpansionQuery::GetQueryDynamicOutput();      }
template<class NavTagTraversePredicate>
KY_INLINE const WorldIntegerPos&   DiskExpansionQuery<NavTagTraversePredicate>::GetCenterIntegerPos()        const { return BaseDiskExpansionQuery::GetCenterIntegerPos();        }
template<class NavTagTraversePredicate>
KY_INLINE const PositionSpatializationRange& DiskExpansionQuery<NavTagTraversePredicate>::GetPositionSpatializationRange()  const { return BaseDiskExpansionQuery::GetPositionSpatializationRange();  }

template<class NavTagTraversePredicate>
KY_INLINE void DiskExpansionQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}


template<class NavTagTraversePredicate>
void DiskExpansionQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != DISKEXPANSION_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	if (m_database->IsClear())
	{
		SetResult(DISKEXPANSION_DONE_CENTER_OUTSIDE);
		return;
	}

	if (workingMemory == KY_NULL)
		workingMemory = m_database->GetWorkingMemory();

	const DatabaseGenMetrics& databaseGenMetrics = m_database->GetDatabaseGenMetrics();
	databaseGenMetrics.GetWorldIntegerPosFromVec3f(m_centerPos3f, m_centerIntegerPos);

	return PerformQueryWithInputCoordPos(workingMemory);
}

template<class NavTagTraversePredicate>
void DiskExpansionQuery<NavTagTraversePredicate>::PerformQueryWithInputCoordPos(WorkingMemory* workingMemory)
{
	KY_DEBUG_ASSERTN(GetResult() == DISKEXPANSION_NOT_PROCESSED, ("Query has not been correctly initialized"));

	QueryUtils queryUtils(m_database, workingMemory);
	TraversalResult traversalRC;

	if (queryUtils.FindTriangleFromPositionIfNotValid(m_centerTrianglePtr, m_centerPos3f, m_centerIntegerPos, m_positionSpatializationRange) == false)
	{
		m_resultRadius = 0.f;
		SetResult(DISKEXPANSION_DONE_CENTER_OUTSIDE);
		return;
	}

	const NavTriangleRawPtr centerTriangleRawPtr = m_centerTrianglePtr.GetRawPtr();

	if(m_predicate.CanTraverse(centerTriangleRawPtr.GetNavTag()) == false)
	{
		m_resultRadius = 0.f;
		SetResult(DISKEXPANSION_DONE_CENTER_NAVTAG_FORBIDDEN);
		return;
	}

	const KyInt32 radiusCellSize = (KyInt32)(m_database->GetDatabaseGenMetrics().GetNearestInteger64FromFloatValue(m_radiusMax) / m_database->GetDatabaseGenMetrics().m_cellSizeInCoord) + 1;
	const CellPos minCellPos(centerTriangleRawPtr.GetCellPos().x - radiusCellSize, centerTriangleRawPtr.GetCellPos().y - radiusCellSize);
	const CellPos maxCellPos(centerTriangleRawPtr.GetCellPos().x + radiusCellSize, centerTriangleRawPtr.GetCellPos().y + radiusCellSize);
	const CellBox cellBox(minCellPos, maxCellPos);

#if defined (KY_BUILD_DEBUG)
	const CoordBox64 cellsIntegerBox(m_database->GetDatabaseGenMetrics().ComputeCellOrigin(minCellPos), m_database->GetDatabaseGenMetrics().ComputeCellOrigin(maxCellPos));
	if (Max(cellsIntegerBox.Max().x - cellsIntegerBox.Min().x, cellsIntegerBox.Max().y - cellsIntegerBox.Min().y) >= (1LL << 31))
		KY_DEBUG_WARNINGN( ("startPos and destPos are too far from each other, overflow may occur during computation, result may be incoherent"));
#endif

	// Set up the visitor and the traversal
	DiskIntersector diskIntersector(*this);
	BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, DiskIntersector> edgeIntersectionVisitor(m_predicate, diskIntersector);
	BestFirstSearchTraversal<BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, DiskIntersector> > traversalForDiskExanpansion(queryUtils, cellBox,
		edgeIntersectionVisitor);

	// set the root node : the triangle that contains m_startPos
	// Not that its navTag is not test within this function, but it has already been tested
	traversalRC = traversalForDiskExanpansion.SetStartTriangle(centerTriangleRawPtr);
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskExpansionResult(traversalRC));
		return;
	}

	WorkingMemArray<NavTriangleRawPtr> visitedTriangles;
	ScopeAutoSaveDynamicOutput scopeAutoSaveDynOutput(m_queryDynamicOutput);
	if ((GetDynamicOutputMode() & QUERY_SAVE_TRIANGLES) != 0)
	{
		visitedTriangles.Init(workingMemory);
		if (visitedTriangles.IsInitialized() == false)
		{
			SetResult(DISKEXPANSION_DONE_LACK_OF_WORKING_MEMORY);
			return;
		}

		scopeAutoSaveDynOutput.SetNavTriangleRawPtrs(&visitedTriangles);
		traversalForDiskExanpansion.SetVisitedNodeContainer(&visitedTriangles);
	}

	// run the traversal algorithm
	traversalRC = traversalForDiskExanpansion.Search();
	if (traversalRC != TraversalResult_DONE)
	{
		SetResult(ConvertTraversalResultToDiskExpansionResult(traversalRC));
		return;
	}

	// retrieve the result of the traversal
	if (edgeIntersectionVisitor.m_collisionFound)
	{
		const NavHalfEdgeRawPtr& intersectedHalfEdgeRawPtr = edgeIntersectionVisitor.m_intersectedHalfEdgeRawPtr;
		m_collisionHalfEdgePtr.Set(intersectedHalfEdgeRawPtr.GetNavFloor(), intersectedHalfEdgeRawPtr.GetHalfEdgeIdx());
		m_collisionPos3f = edgeIntersectionVisitor.m_collisionPos;

		const NavTriangleRawPtr triangleRawPtr(intersectedHalfEdgeRawPtr.GetNavFloor(), NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(intersectedHalfEdgeRawPtr.GetHalfEdgeIdx()));
		m_collisionPos3f.z = triangleRawPtr.GetAltitudeOfPointInTriangle(m_collisionPos3f);

		m_resultRadius = (m_collisionPos3f - m_centerPos3f).GetLength2d() - m_safetyDist;

		if (m_resultRadius < 0.f)
		{
			m_resultRadius = 0.f;
			SetResult(DISKEXPANSION_DONE_CANNOT_EXPAND);
		}
		else
		{
			SetResult(DISKEXPANSION_DONE_COLLISION_FOUND);
		}
	}
	else
	{
		m_resultRadius = m_radiusMax;
		SetResult(DISKEXPANSION_DONE_DISK_FIT);
	}
}

}
