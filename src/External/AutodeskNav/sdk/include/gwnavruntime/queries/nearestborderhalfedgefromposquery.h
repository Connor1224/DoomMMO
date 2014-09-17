/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/




// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NearestBorderHalfEdgeFromPosQuery_H
#define Navigation_NearestBorderHalfEdgeFromPosQuery_H

#include "gwnavruntime/queries/utils/basenearestborderhalfedgefromposquery.h"
#include "gwnavruntime/queries/utils/querycommon.h"
#include "gwnavruntime/navmesh/closestpoint.h"
#include "gwnavruntime/navmesh/intersections.h"
#include "gwnavruntime/database/database.h"
#include "gwnavruntime/database/activedata.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmemarray.h"

namespace Kaim
{
/// The NearestBorderHalfEdgeFromPosQuery class finds the NavMesh border nearest a specified 3D position (#m_inputPos3f).
/// The input position may be inside or outside the NavMesh boundaries (by boundary we mean hole in the NavMesh or forbidden NavTag boundary).
/// The search space is constrained within an axis-aligned bounding box that is centered on #m_inputPos3f. The horizontal width of the bounding box is equal to
/// 2 * #m_horizontalTolerance along both the X and Y axes. The vertical height of the bounding box is retrieved from #m_positionSpatializationRange.
/// If a NavMesh border is found within the bounding box, its NavHalfEdgePtr is written to #m_nearestHalfEdgeOnBorder, and the nearest point on this edge
/// is written to #m_nearestPosOnHalfEdge.
template<class NavTagTraversePredicate>
class NearestBorderHalfEdgeFromPosQuery : public BaseNearestBorderHalfEdgeFromPosQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	NearestBorderHalfEdgeFromPosQuery();
	NearestBorderHalfEdgeFromPosQuery(const NavTagTraversePredicate& predicate);
	virtual ~NearestBorderHalfEdgeFromPosQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database    A pointer to the Database on which the query must be performed. Sets the value of m_database
	/// \param inputPos    Sets the value of #m_inputPos3f. 
	void Initialize(Database* database, const Vec3f& inputPos);

	/// Write accessor for #m_horizontalTolerance. 
	void SetHorizontalTolerance(KyFloat32 horizontalTolerance);

	/// Write accessor for #m_positionSpatializationRange. 
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	// ---------------------------------- Query framework functions ----------------------------------

	virtual void Advance(WorkingMemory* workingMemory);

	/// Performs the query.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                          the #m_database internal WorkingMemory is used.
	void PerformQuery(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------

	NearestBorderHalfEdgeFromPosQueryResult GetResult()                      const;
	const Vec3f&                            GetInputPos()                    const;
	const Vec3f&                            GetNearestPosOnHalfEdge()        const;
	const NavHalfEdgePtr&                   GetNearestHalfEdgePtrOnBorder()  const;
	const NavHalfEdgePtr&                   GetNextHalfEdgePtrAlongBorder()  const;
	const NavHalfEdgePtr&                   GetPrevHalfEdgePtrAlongBorder()  const;
	KyFloat32                               GetHorizontalTolerance()         const;
	KyFloat32                               GetSquareDistFromNearestBorder() const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetInputIntegerPos(const WorldIntegerPos& inputIntegerPos);
	const WorldIntegerPos& GetInputIntegerPos() const;
	const WorldIntegerPos& GetNearestIntegerPosOnHalfEdge() const;

private:
	void FindNearestBorderHalfEdgeInFloor(NavFloor* navFloor, const CoordBox64& aABbox, const CoordPos64& inputCoordPos, const CoordPos64& cellOrigin, NavHalfEdgeRawPtr& nearestHalfEdgeRawPtr);

public:
	NavTagTraversePredicate m_predicate;
};


}

#include "gwnavruntime/queries/nearestborderhalfedgefromposquery.inl"

#endif //Navigation_NearestBorderHalfEdgeFromPosQuery_H

