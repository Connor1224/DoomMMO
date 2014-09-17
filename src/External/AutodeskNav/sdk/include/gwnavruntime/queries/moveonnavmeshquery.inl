/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

KY_INLINE MoveOnNavMeshQuery::MoveOnNavMeshQuery() :
	m_startPos3f(KyFloat32MAXVAL, KyFloat32MAXVAL, KyFloat32MAXVAL),
	m_normalizedDir2d(KyFloat32MAXVAL, KyFloat32MAXVAL),
	m_distanceToBoundary(0.02f),
	m_distance(0.f),
	m_arrivalPos3f(KyFloat32MAXVAL, KyFloat32MAXVAL, KyFloat32MAXVAL),
	m_result(MOVEONNAVMESH_NOT_INITIALIZED)
	{}

KY_INLINE void MoveOnNavMeshQuery::Initialize(Database* database, const Vec3f& startPos, const Vec2f& normalizedDir2d, KyFloat32 maxDist)
{
	IQuery::Initialize(database);

	m_startPos3f = startPos;
	m_normalizedDir2d = normalizedDir2d;
	m_distance = maxDist;

	m_arrivalPos3f.Set(KyFloat32MAXVAL, KyFloat32MAXVAL, KyFloat32MAXVAL);
	m_startTrianglePtr.Invalidate();

	SetResult(MOVEONNAVMESH_NOT_PROCESSED);
}

KY_INLINE void MoveOnNavMeshQuery::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

KY_INLINE void MoveOnNavMeshQuery::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr) { m_startTrianglePtr  = startTrianglePtr; }
KY_INLINE void MoveOnNavMeshQuery::SetResult(MoveOnNavMeshQueryResult result)                  { m_result            = result;           }

KY_INLINE MoveOnNavMeshQueryResult MoveOnNavMeshQuery::GetResult()           const { return m_result;           }
KY_INLINE const Vec3f&             MoveOnNavMeshQuery::GetStartPos()         const { return m_startPos3f;       }
KY_INLINE const Vec3f&             MoveOnNavMeshQuery::GetArrivalPos()       const { return m_arrivalPos3f;     }
KY_INLINE const Vec2f&             MoveOnNavMeshQuery::GetNormalizedDir2d()  const { return m_normalizedDir2d;  }
KY_INLINE const NavTrianglePtr&    MoveOnNavMeshQuery::GetStartTrianglePtr() const { return m_startTrianglePtr; }
KY_INLINE KyFloat32                MoveOnNavMeshQuery::GetDistance()         const { return m_distance;         }

}
