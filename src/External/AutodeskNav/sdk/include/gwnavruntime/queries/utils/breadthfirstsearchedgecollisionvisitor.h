/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_BreadthFirstSearchEdgeCollisionVisitor_H
#define Navigation_BreadthFirstSearchEdgeCollisionVisitor_H

#include "gwnavruntime/querysystem/workingmemcontainers/trianglestatusingrid.h"

namespace Kaim
{

/*
class BreadthFirstSearchEdgeCollisionVisitor

EdgeIntersector must have the following function :

	bool DoesIntersectEdge(const Vec3f& startEdgePos, const Vec3f& endEdgePos)
*/
template<class NavTagTraversePredicate, class EdgeIntersector>
class BreadthFirstSearchEdgeCollisionVisitor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Query)

public:
	BreadthFirstSearchEdgeCollisionVisitor(const NavTagTraversePredicate& predicate, EdgeIntersector& edgeIntersector);

	bool IsSearchFinished();
	bool ShouldVisitNeighborTriangle(const NavTriangleRawPtr& triangleRawPtr, KyUInt32 indexOfNeighborTriangle);
	bool ShouldVisitTriangle(const NavTriangleRawPtr& triangleRawPtr);

	NavTriangleRawPtr GetNeighborTriangle(const NavTriangleRawPtr& triangleRawPtr, KyUInt32 indexOfNeighborTriangle);

	void Visit(const NavTriangleRawPtr& triangleRawPtr, const TriangleStatusInGrid& triangleStatus);

public:
	EdgeIntersector* m_edgeIntersector;
	NavTagTraversePredicate m_predicate;
	bool m_collisionFound;

private:
	bool m_shouldVisitNeighbourTriangle[3];
	NavTriangleRawPtr m_neighbourTriangle[3];
};

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::
	BreadthFirstSearchEdgeCollisionVisitor(const NavTagTraversePredicate& predicate, EdgeIntersector& edgeIntersector) :
	m_edgeIntersector(&edgeIntersector),
	m_predicate(predicate),
	m_collisionFound(false)
{
	m_shouldVisitNeighbourTriangle[0] = true;
	m_shouldVisitNeighbourTriangle[1] = true;
	m_shouldVisitNeighbourTriangle[2] = true;
}

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::IsSearchFinished() { return m_collisionFound; }
template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::ShouldVisitNeighborTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/, KyUInt32 indexOfNeighborTriangle)
{
	return m_shouldVisitNeighbourTriangle[indexOfNeighborTriangle];
}

template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE bool BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::ShouldVisitTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/) { return true; }
template<class NavTagTraversePredicate, class EdgeIntersector>
KY_INLINE NavTriangleRawPtr BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::GetNeighborTriangle(const NavTriangleRawPtr& /*triangleRawPtr*/, KyUInt32 indexOfNeighborTriangle)
{
	return m_neighbourTriangle[indexOfNeighborTriangle];
}

template<class NavTagTraversePredicate, class EdgeIntersector>
void BreadthFirstSearchEdgeCollisionVisitor<NavTagTraversePredicate, EdgeIntersector>::Visit(const NavTriangleRawPtr& triangleRawPtr,
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

	m_collisionFound =
		(isACrossableEdges[0] == false && doesIntersectsEdges[0]) ||
		(isACrossableEdges[1] == false && doesIntersectsEdges[1]) ||
		(isACrossableEdges[2] == false && doesIntersectsEdges[2]);

	for(KyUInt32 i = 0; i < 3; ++i)
		m_shouldVisitNeighbourTriangle[i] = isACrossableEdges[i] && neighbourTriangleAlreadyVisited[i] == false && doesIntersectsEdges[i];
}

}


#endif //Navigation_BreadthFirstSearchEdgeCollisionVisitor_H

