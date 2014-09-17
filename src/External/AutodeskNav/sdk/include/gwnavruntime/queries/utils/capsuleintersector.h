/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_CapsuleIntersector_H
#define Navigation_CapsuleIntersector_H

#include "gwnavruntime/database/database.h"
#include "gwnavruntime/navmesh/intersections.h"
#include "gwnavruntime/navmesh/closestpoint.h"
#include "gwnavruntime/queries/utils/basediskcangoquery.h"
#include "gwnavruntime/queries/utils/basediskcastquery.h"


namespace Kaim
{

/*
class CapsuleIntersector
*/
class CapsuleIntersector
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Query)

public:
	CapsuleIntersector(const BaseDiskCanGoQuery& query) :
		m_startCenter(query.GetStartPos()), m_endCenter(query.GetDestPos()), m_radius(query.GetRadius())
	{
		m_integerPrecision = query.m_database->GetDatabaseGenMetrics().m_integerPrecision;

		Vec3f dir(query.GetDestPos() - query.GetStartPos());
		dir.z = 0.f;
		m_dist = dir.Normalize();

		const Vec3f normalizedDir3D_ortho(-dir.y, dir.x, 0.f);
		const Vec3f box_a(m_startCenter - (normalizedDir3D_ortho * m_radius));

		m_orientedBox2d.Set(box_a, Vec2f(dir.x, dir.y), m_dist, m_radius * 2.f, 0.f);
	}

	CapsuleIntersector(const BaseDiskCastQuery& query) : m_startCenter(query.GetStartPos()),
		m_normalizedDir2d(query.GetNormalizedDir2d()), m_dist(query.GetMaxDist()), m_radius(query.GetRadius())
	{
		m_integerPrecision = query.m_database->GetDatabaseGenMetrics().m_integerPrecision;

		const Vec3f normalizedDir3D_ortho(-query.GetNormalizedDir2d().y, query.GetNormalizedDir2d().x, 0.f);
		const Vec3f box_a(m_startCenter - (normalizedDir3D_ortho * m_radius));
		m_orientedBox2d.Set(box_a, m_normalizedDir2d, m_dist, m_radius * 2.f, 0.f);

		const Vec3f normalizedDir3D(m_normalizedDir2d.x, m_normalizedDir2d.y, 0.f);
		m_endCenter = m_startCenter + (normalizedDir3D * m_dist);
	}

	bool DoesIntersectEdge(const CoordPos64& startEdgeCoordPos, const CoordPos64& endEdgeCoordPos)
	{
		const Vec3f startEdgePos(startEdgeCoordPos.x * m_integerPrecision, startEdgeCoordPos.y * m_integerPrecision, 0.f);
		const Vec3f endEdgePos(endEdgeCoordPos.x * m_integerPrecision, endEdgeCoordPos.y * m_integerPrecision, 0.f);

		return Intersections::SegmentVsCapsule2d(startEdgePos, endEdgePos, m_orientedBox2d, m_startCenter, m_endCenter, m_radius);
	}

	void ComputeCollisionPosOnEdge(const CoordPos64& startEdgeCoordPos, const CoordPos64& endEdgeCoordPos, Vec3f& collisionPos, KyFloat32& squareDistToCollisionPos)
	{
		const Vec3f startEdgePos(startEdgeCoordPos.x * m_integerPrecision, startEdgeCoordPos.y * m_integerPrecision, 0.f);
		const Vec3f endEdgePos(endEdgeCoordPos.x * m_integerPrecision, endEdgeCoordPos.y * m_integerPrecision, 0.f);

		ClosestPoint::OnSegmentVsDisk2d(startEdgePos, endEdgePos, m_startCenter, m_radius, m_normalizedDir2d, m_dist, collisionPos, squareDistToCollisionPos);
	}

	void ComputeTriangleCost(const CoordPos64& v0CoordPos, const CoordPos64& v1CoordPos, const CoordPos64& v2CoordPos, KyFloat32& cost)
	{
		const Vec3f v0(v0CoordPos.x * m_integerPrecision, v0CoordPos.y * m_integerPrecision, 0.f);
		const Vec3f v1(v1CoordPos.x * m_integerPrecision, v1CoordPos.y * m_integerPrecision, 0.f);
		const Vec3f v2(v2CoordPos.x * m_integerPrecision, v2CoordPos.y * m_integerPrecision, 0.f);

		Vec3f unused;
		ClosestPoint::OnTriangleVsDisk2d(v0, v1, v2, m_startCenter, m_radius, m_normalizedDir2d, m_dist, unused, cost);
	}

private:
	KyFloat32 m_integerPrecision;

	Vec3f m_startCenter;
	Vec3f m_endCenter;
	Vec2f m_normalizedDir2d;
	KyFloat32 m_dist;
	KyFloat32 m_radius; //< the radius of the extrimity disks

	OrientedBox2d m_orientedBox2d;
};


}


#endif //Navigation_CapsuleIntersector_H

