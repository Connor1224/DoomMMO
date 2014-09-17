/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: LAPA
#ifndef Navigation_MoveOnNavMeshQuery_H
#define Navigation_MoveOnNavMeshQuery_H

#include "gwnavruntime/querysystem/iquery.h"
#include "gwnavruntime/queries/utils/querycommon.h"
#include "gwnavruntime/navmesh/identifiers/navhalfedgeptr.h"

namespace Kaim
{

class NavHalfEdgeRawPtr;
template<class NavTagTraversePredicate> class RayCastQuery;

/// Enumerates the possible results of a MoveOnNavMeshQuery.
enum MoveOnNavMeshQueryResult
{
	MOVEONNAVMESH_NOT_INITIALIZED = QUERY_NOT_INITIALIZED, ///< Indicates the query has not yet been initialized. 
	MOVEONNAVMESH_NOT_PROCESSED   = QUERY_NOT_PROCESSED,   ///< Indicates the query has not yet been launched. 

	MOVEONNAVMESH_DONE_ARRIVALPOS_NOT_FOUND,               ///< Indicates a collision point was found, but a final arrival position was not successfully calculated. 
	MOVEONNAVMESH_DONE_ARRIVALPOS_FOUND_COLLISION,         ///< Indicates a collision point was found, and a final arrival position was successfully calculated. 
	MOVEONNAVMESH_DONE_ARRIVALPOS_FOUND_NO_COLLISION       ///< Indicates an arrival point was successfully calculated without a collision with the NavMesh border. 
};

/// This class is used to get move bots on the NavMesh without physics engine.
class MoveOnNavMeshQuery : public IAtomicQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	static  QueryType GetStaticType() { return TypeMoveOnNavMesh; }
	virtual QueryType GetType() const { return TypeMoveOnNavMesh; }

	MoveOnNavMeshQuery();
	virtual ~MoveOnNavMeshQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database           Specifies the Database the query will be run against. 
	/// \param startPos           Sets the value of #m_startPos3f. 
	/// \param normalizedDir2d    Sets the value of #m_normalizedDir2d.
	/// \param maxDist            Sets the value of #m_distance. 
	void Initialize(Database* database, const Vec3f& startPos, const Vec2f& normalizedDir2d, KyFloat32 maxDist);

	/// Set The NavMesh triangle that corresponds to #m_startPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query. 
	/// Call this method after Initialize(), during which #m_startTrianglePtr is set to its default value.
	void SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr);

	// ---------------------------------- Query framework functions ----------------------------------

	virtual void Advance(WorkingMemory* workingMemory);

	/// Performs the query.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                          the #m_database internal WorkingMemory is used.
	void PerformQuery(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------

	MoveOnNavMeshQueryResult GetResult()            const;
	const Vec3f&             GetStartPos()          const;
	const Vec3f&             GetArrivalPos()        const;
	const Vec2f&             GetNormalizedDir2d()   const;
	const NavTrianglePtr&    GetStartTrianglePtr()  const;
	KyFloat32                GetDistance()          const;
	KyFloat32                GetAltitudeTolerance() const;

private:
	void SetResult(MoveOnNavMeshQueryResult result);
	void ComputeArrivalPosFromRayCastCollision(WorkingMemory* workingMemory, RayCastQuery<IgnoreNavTagPredicate>& rayCastQuery);
private:
	Vec3f m_startPos3f;      ///< The starting point of the line segment being tested.
	Vec2f m_normalizedDir2d; ///< A normalized vector representing the direction the ray is cast.

	NavTrianglePtr m_startTrianglePtr; ///< The NavMesh triangle corresponding to #m_startPos3f. It can be an input, or it can be retrieved as an output of the query.

	/// The minimal distance to NavMesh boundary the resulting position should be. When the movement hits a boundary, we move back
	/// the final position so that it is at #m_distanceToBoundary meters from this boundary.
	KyFloat32 m_distanceToBoundary;

	/// Determines the maximum distance the disk will travel.
	/// If this distance is too far from the starting point (approximately 32 times the length of a NavData cell), running
	/// the query can result in an integer overflow during calculation.
	KyFloat32 m_distance;

	/// Updated during processing to indicate the final position of the ray at the end of the cast.
	/// -  If no collision was detected, the result is the triangle arrived at by traversing the NavMesh in the direction of #m_normalizedDir2d.
	/// -  If a collision is detected, the result is derived from the collision point by backtracking along the orientation
	///     of #m_normalizedDir2d. This ensures #m_arrivalPos3f will be inside the boundaries of the NavMesh.
	Vec3f m_arrivalPos3f;

	MoveOnNavMeshQueryResult m_result; ///< Updated during processing to indicate the result of the query.
};

}

#include "gwnavruntime/queries/moveonnavmeshquery.inl"



#endif //Navigation_MoveOnNavMeshQuery_H

