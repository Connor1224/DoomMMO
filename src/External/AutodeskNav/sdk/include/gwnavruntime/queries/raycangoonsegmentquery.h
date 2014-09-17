/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_RayCanGoOnSegmentQuery_H
#define Navigation_RayCanGoOnSegmentQuery_H


#include "gwnavruntime/queries/utils/baseraycangoonsegmentquery.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmemarray.h"

namespace Kaim
{

class QueryUtils;
class NavHalfEdgeRawPtr;
class RawNavTagSubSegment;

/// The RayCanGoOnSegmentQuery class tests whether or not a ray (with a width of 0) can pass along a straight line segment from a
/// along an edge going from #m_segmentStartPos3f to #m_segmentEndPos3f, without :
/// -	a collision with the outside border of the NavMesh.
/// -	crossing a forbidden NavTag (a triangle whose NavTag is considered as non-traversable by the predicate).
template<class NavTagTraversePredicate>
class RayCanGoOnSegmentQuery : public BaseRayCanGoOnSegmentQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	RayCanGoOnSegmentQuery();
	RayCanGoOnSegmentQuery(const NavTagTraversePredicate& predicate);
	virtual ~RayCanGoOnSegmentQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database            Specifies the Database this query will be run against. 
	/// \param startPos            Sets the value of #m_startPos3f. 
	/// \param startTrianglePtr    Sets the value of #m_startTrianglePtr. 
	/// \param segmentStartPos     Sets the value of #m_segmentStartPos3f. 
	/// \param segmentEndPos       Sets the value of #m_segmentEndPos3f. 
	void Initialize(Database* database, const Vec3f& startPos, const NavTrianglePtr& startTrianglePtr, const Vec3f& segmentStartPos, const Vec3f& segmentEndPos);

	/// Write accessor for #m_positionSpatializationRange.
	/// Vall this method after Initialize(), during which #m_positionSpatializationRange is set to its default value. 
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	/// Write accessor for #m_dynamicOutputMode.
	/// Call this method after Initialize(), during which #m_dynamicOutputMode is set to its default value. 
	void SetDynamicOutputMode(DynamicOutputMode savingMode);

	/// Write accessor for #m_queryDynamicOutput.
	/// Call this method after Initialize(), during which #m_queryDynamicOutput is set to KY_NULL.
	void SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput);

	// ---------------------------------- Query framework functions ----------------------------------

	virtual void Advance(WorkingMemory* workingMemory);

	/// Performs the query.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                          the #m_database internal WorkingMemory is used.
	void PerformQuery(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------

	RayCanGoOnSegmentQueryResult GetResult()             const;
	const Vec3f&                 GetStartPos()           const;
	const Vec3f&                 GetSegmentStartPos()    const;
	const Vec3f&                 GetSegmentEndPos()      const;
	const NavTrianglePtr&        GetStartTrianglePtr()   const;
	const NavTrianglePtr&        GetArrivalTrianglePtr() const;
	DynamicOutputMode            GetDynamicOutputMode()  const;
	QueryDynamicOutput*          GetQueryDynamicOutput() const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetStartIntegerPos(const WorldIntegerPos& startIntegerPos);
	void SetSegmentStartIntegerPos(const WorldIntegerPos& segmentStartIntegerPos);
	void SetSegmentEndIntegerPos(const WorldIntegerPos& segmentEndIntegerPos);
	const WorldIntegerPos& GetStartIntegerPos() const;
	const WorldIntegerPos& GetSegmentStartIntegerPos() const;
	const WorldIntegerPos& GetSegmentEndIntegerPos() const;

public:
	NavTagTraversePredicate m_predicate;
};

}

#include "gwnavruntime/queries/raycangoonsegmentquery.inl"



#endif //Navigation_RayCanGoOnSegmentQuery_H

