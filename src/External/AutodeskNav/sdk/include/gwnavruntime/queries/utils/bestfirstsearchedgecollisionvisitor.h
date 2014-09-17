/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_BestFirstSearchEdgeCollisionVisitor_H
#define Navigation_BestFirstSearchEdgeCollisionVisitor_H

#include "gwnavruntime/querysystem/workingmemcontainers/trianglestatusingrid.h"

namespace Kaim
{

/*
class BestFirstSearchEdgeCollisionVisitor

EdgeIntersector must have the following functions :

	bool DoesIntersectEdge(const Vec3f& startEdgePos, const Vec3f& endEdgePos);

	void ComputeCollisionPosOnEdge(const Vec3f& startEdgePos, const Vec3f& endEdgePos, Vec3f& collisionPos, KyFloat32& squareDistToCollisionPos)

	void ComputeTriangleCost(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, KyFloat32& cost)

*/
template<class NavTagTraversePredicate, class EdgeIntersector>
class BestFirstSearchEdgeCollisionVisitor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Query)

public:
	BestFirstSearchEdgeCollisionVisitor(const NavTagTraversePredicate& predicate, EdgeIntersector& edgeIntersector);

	bool IsSearchFinished();

	bool ShouldVisitNeighborTriangle(const NavTriangleRawPtr& triangleRawPtr, KyUInt32 indexOfNeighborTriangle);
	bool ShouldVisitTriangle(const NavTriangleRawPtr& triangleRawPtr);
	void ComputeTriangleCost(const NavTriangleRawPtr& triangleRawPtr, KyFloat32& cost);

	NavTriangleRawPtr GetNeighborTriangle(const NavTriangleRawPtr& triangleRawPtr, KyUInt32 indexOfNeighborTriangle);

	void Visit(const NavTriangleRawPtr& triangleRawPtr, KyFloat32 cost, const TriangleStatusInGrid& triangleStatus);

public:
	EdgeIntersector* m_edgeIntersector;
	NavTagTraversePredicate m_predicate;
	bool m_collisionFound;

	KyFloat32 m_squareDistToCollisionPos;
	Vec3f m_collisionPos;
	NavHalfEdgeRawPtr m_intersectedHalfEdgeRawPtr;

private:
	KyFloat32 m_lastCost;
	bool m_shouldVisitNeighbourTriangle[3];
	NavTriangleRawPtr m_neighbourTriangle[3];
};

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::BestFirstSearchEdgeCollisionVisitor(const NavTagTraversePredicate& predicate, EdgeIntersector& edgeIntersector) :
m_edgeIntersector(&edgeIntersector), m_predicate(predicate), m_collisionFound(false),
	m_squareDistToCollisionPos(KyFloat32MAXVAL), m_lastCost(KyFloat32MAXVAL)
{
	m_shouldVisitNeighbourTriangle[0] = true;
	m_shouldVisitNeighbourTriangle[1] = true;
	m_shouldVisitNeighbourTriangle[2] = true;
}

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::IsSearchFinished() { return m_lastCost > m_squareDistToCollisionPos; }

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::ShouldVisitNeighborTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/, KyUInt32 indexOfNeighborTriangle)
{
	return m_shouldVisitNeighbourTriangle[indexOfNeighborTriangle];
}

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::ShouldVisitTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/) { return true; }

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE NavTriangleRawPtr BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::GetNeighborTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/, KyUInt32 indexOfNeighborTriangle)
{
	return m_neighbourTriangle[indexOfNeighborTriangle];
}

template<class NavTagTraversePredicate, class EdgeIntersector>
void BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::ComputeTriangleCost(const NavTriangleRawPtr& triangleRawPtr, KyFloat32& cost)
{
	CoordPos64 v0, v1, v2;
	triangleRawPtr.GetVerticesCoordPos64(v0, v1, v2);
	m_edgeIntersector->ComputeTriangleCost(v0, v1, v2, cost);
}

template<class NavTagTraversePredicate, class EdgeIntersector>
void BestFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::Visit(const NavTriangleRawPtr& triangleRawPtr, const KyFloat32 triangleCost,
	const TriangleStatusInGrid& triangleStatus)
{
	CoordPos64 v0, v1, v2;
	CoordPos64* vertex[4] = { &v0, &v1, &v2, &v0};
	NavHalfEdgeRawPtr halfEdgeRawPtr[3];
	NavHalfEdge halfEdge[3];
	bool isACrossableEdges[3];
	bool neighbourTriangleAlreadyVisited[3];
	bool doesIntersectsEdges[3];

	const NavHalfEdgeIdx firstNavHalfEdgeIdx = NavFloorBlob::NavTriangleIdxToFirstNavHalfEdgeIdx(triangleRawPtr.GetTriangleIdx());
	NavFloor* navFloor = triangleRawPtr.GetNavFloor();

	for(KyUInt32 i = 0; i < 3; ++i)
		halfEdgeRawPtr[i].Set(navFloor, firstNavHalfEdgeIdx + i);

	triangleRawPtr.GetVerticesCoordPos64(v0, v1, v2);

	const NavFloorBlob& navFloorBlob = *navFloor->GetNavFloorBlob();
	const NavHalfEdge* m_navHalfEdges = navFloorBlob.m_navHalfEdges.GetValues();

	for(KyUInt32 i = 0; i < 3; ++i)
		halfEdge[i] = m_navHalfEdges[firstNavHalfEdgeIdx + i];

	for(KyUInt32 i = 0; i < 3; ++i)
	{
		NavHalfEdgeRawPtr pairHalfEdgeRawPtr;
		isACrossableEdges[i] = halfEdgeRawPtr[i].IsHalfEdgeCrossable<NavTagTraversePredicate>(halfEdge[i], &navFloorBlob, pairHalfEdgeRawPtr, &m_predicate);

		if (isACrossableEdges[i])
		{
			bool inSameFloor = halfEdge[i].GetHalfEdgeType() == EDGETYPE_PAIRED || halfEdge[i].GetHalfEdgeType() == EDGETYPE_CONNEXBOUNDARY;
			m_neighbourTriangle[i].Set(pairHalfEdgeRawPtr.GetNavFloor(), NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(pairHalfEdgeRawPtr.GetHalfEdgeIdx()));

			if (inSameFloor)
				neighbourTriangleAlreadyVisited[i] = triangleStatus.IsTriangleOpen_Unsafe(m_neighbourTriangle[i]);
			else
				neighbourTriangleAlreadyVisited[i] = triangleStatus.IsTriangleOpen(m_neighbourTriangle[i]);
		}
		else
		{
			neighbourTriangleAlreadyVisited[i] = false;
		}
	}

	for(KyUInt32 i = 0; i < 3; ++i)
		doesIntersectsEdges[i] = neighbourTriangleAlreadyVisited[i] || m_edgeIntersector->DoesIntersectEdge(*vertex[i], *vertex[i+1]);

	m_lastCost = triangleCost;

	if (isACrossableEdges[0] == false || isACrossableEdges[1] == false || isACrossableEdges[2] == false)
	{
		Vec3f collisionPos;
		KyFloat32 squareDistToCollisionPos;

		bool aCollisionFoundIsThisTriangle = false;

		for(KyUInt32 i = 0; i < 3; ++i)
		{
			if (isACrossableEdges[i] == false && doesIntersectsEdges[i])
			{
				m_edgeIntersector->ComputeCollisionPosOnEdge(*vertex[i], *vertex[i+1], collisionPos, squareDistToCollisionPos);
				if (squareDistToCollisionPos < m_squareDistToCollisionPos)
				{
					m_squareDistToCollisionPos = squareDistToCollisionPos;
					aCollisionFoundIsThisTriangle = true;
					m_collisionPos = collisionPos;
					m_intersectedHalfEdgeRawPtr.Set(triangleRawPtr.m_navFloorRawPtr, NavFloorBlob::NavTriangleIdxToNavHalfEdgeIdx(triangleRawPtr.GetTriangleIdx(), i));
				}
			}
		}
		
		m_collisionFound = m_collisionFound || aCollisionFoundIsThisTriangle;
	}

	for(KyUInt32 i = 0; i < 3; ++i)
		m_shouldVisitNeighbourTriangle[i] = isACrossableEdges[i] && neighbourTriangleAlreadyVisited[i] == false && doesIntersectsEdges[i];
}

}


#endif //Navigation_BestFirstSearchEdgeCollisionVisitor_H

