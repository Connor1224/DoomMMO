/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_InsidePosFromOutsidePosQuery_H
#define Navigation_InsidePosFromOutsidePosQuery_H

#include "gwnavruntime/queries/utils/baseinsideposfromoutsideposquery.h"
#include "gwnavruntime/queries/nearestborderhalfedgefromposquery.h"
#include "gwnavruntime/queries/raycastquery.h"
#include "gwnavruntime/basesystem/iperfmarkerinterface.h"


namespace Kaim
{

/// The InsidePosFromOutsidePosQuery class finds a valid position inside the boundaries of the NavMesh, nearest to a specified input position (#m_inputPos3f), at a specified distance
/// from the NavMesh boundary (by boundary we mean hole in the NavMesh or forbidden NavTag boundary).
/// This query is typically used to retrieve a valid NavMesh position as near as possible to an input position lying outside the 
/// (X,Y) boundaries of the NavMesh. It can also be used with an input position that is inside the NavMesh. In either case,
/// it finds the NavMesh boundary nearest to the specified point, then finds the point closest to the input position at the
/// specified distance inside that boundary. 
/// The search space is constrained within an axis-aligned bounding box centered on #m_inputPos3f. The horizontal width of the bounding box is equal to
/// 2 * #m_horizontalTolerance along both the X and Y axes. The vertical height of the bounding box is retrieved from #m_positionSpatializationRange.
/// If a NavMesh boundary is found within the search space, and a valid position can be found near that boundary, the position is written to
/// #m_insidePos3f, and the triangle that contains the output position is written to #m_insidePosTrianglePtr.
template<class NavTagTraversePredicate>
class InsidePosFromOutsidePosQuery : public BaseInsidePosFromOutsidePosQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	InsidePosFromOutsidePosQuery();
	InsidePosFromOutsidePosQuery(const NavTagTraversePredicate& predicate);
	virtual ~InsidePosFromOutsidePosQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database    A pointer to the Database on which the query must be performed. Sets the value of m_database.
	/// \param inputPos    Sets the value of #m_inputPos3f. 
	void Initialize(Database* database, const Vec3f& inputPos);

	/// Write accessor for #m_horizontalTolerance. 
	void SetHorizontalTolerance(KyFloat32 horizontalTolerance);

	/// Write accessor for #m_positionSpatializationRange.
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	/// Write accessor for #m_distFromObstacle. 
	void SetDistFromObstacle(KyFloat32 distFromObstacle);

	// ---------------------------------- Query framework functions ----------------------------------

	virtual void Advance(WorkingMemory* workingMemory);

	/// Performs the query.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                          the #m_database internal WorkingMemory is used.
	void PerformQuery(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------

	InsidePosFromOutsidePosQueryResult GetResult()                 const;
	const Vec3f&                       GetInputPos()               const;
	const Vec3f&                       GetInsidePos()              const;
	const NavTrianglePtr&              GetInsidePosTrianglePtr()   const;
	KyFloat32                          GetHorizontalTolerance()    const;
	KyFloat32                          GetDistFromObstacle()       const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetInputIntegerPos(const WorldIntegerPos& inputIntegerPos);
	const WorldIntegerPos& GetInputIntegerPos() const;
	const WorldIntegerPos& GetInsideIntegerPos() const;

public:
	NavTagTraversePredicate m_predicate;
};

}

#include "gwnavruntime/queries/insideposfromoutsideposquery.inl"



#endif //Navigation_InsidePosFromOutsidePosQuery_H

