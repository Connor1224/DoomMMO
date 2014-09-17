/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_DiskCanGoQuery_H
#define Navigation_DiskCanGoQuery_H

#include "gwnavruntime/queries/utils/basediskcangoquery.h"

#include "gwnavruntime/queries/raycangoquery.h"
#include "gwnavruntime/queries/raycastquery.h"
#include "gwnavruntime/queries/diskcollisionquery.h"
#include "gwnavruntime/queries/utils/capsuleintersector.h"
#include "gwnavruntime/queries/utils/crossedsectioncapsuleintersector.h"
#include "gwnavruntime/queries/utils/breadthfirstsearchedgecollisionvisitor.h"
#include "gwnavruntime/queries/utils/breadthfirstsearchtraversal.h"

namespace Kaim
{

class NavTriangleRawPtr;

/// The DiskCanGoQuery tests whether a disk with a specified radius can pass from a starting point to an ending point without:
/// -    a collision with the outside border of the NavMesh,
/// -    crossing a forbidden NavTag (a triangle whose NavTag is considered as non-traversable by the predicate).
template<class NavTagTraversePredicate>
class DiskCanGoQuery : public BaseDiskCanGoQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	DiskCanGoQuery();
	DiskCanGoQuery(const NavTagTraversePredicate& predicate);
	virtual ~DiskCanGoQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessors.
	/// \param database    A pointer to the Database on which the query must be performed. Sets the value of #m_database.
	/// \param startPos    Sets the value of #m_startPos3f.
	/// \param destPos     Sets the value of #m_destPos3f.
	/// \param radius      Sets the value of #m_radius. 
	void Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos, KyFloat32 radius);

	/// The NavMesh triangle that corresponds to #m_startPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query.
	/// Call this method after Initialize(), during which #m_startTrianglePtr is set to its default value. 
	void SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr);

	/// Write accessor for #m_positionSpatializationRange.
	/// Call this method after Initialize(), during which #m_positionSpatializationRange is set to its default value.
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	/// Write accessor for #m_queryType.
	/// Call this method after Initialize(), during which #m_queryType is set to its default value.
	void SetQueryType(DiskCanGoQueryType queryType);

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

	DiskCanGoQueryResult  GetResult()             const;
	const Vec3f&          GetStartPos()           const;
	const Vec3f&          GetDestPos()            const;
	const NavTrianglePtr& GetStartTrianglePtr()   const;
	const NavTrianglePtr& GetDestTrianglePtr()    const;
	KyFloat32             GetRadius()             const;
	DiskCanGoQueryType    GetQueryType()          const;
	DynamicOutputMode     GetDynamicOutputMode()  const;
	QueryDynamicOutput*   GetQueryDynamicOutput() const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetStartIntegerPos(const WorldIntegerPos& startIntegerPos);
	void SetDestIntegerPos(const WorldIntegerPos& destIntegerPos);
	const WorldIntegerPos& GetStartIntegerPos() const;
	const WorldIntegerPos& GetDestIntegerPos() const;

private:
	void ForceDiskCanGo(WorkingMemory* workingMemory, const CellBox& cellBox, const NavTriangleRawPtr& startTriangleRawPtr);
	KyResult RayCastQueryFromStartOnTheRigth(WorkingMemory* workingMemory, const Vec2f& normalizedDir2d, RayCastQuery<NavTagTraversePredicate>& rayCastQuery,
		RayCastQueryResult& queryOnRightResult, KyFloat32& borderDistOnRight);
	KyResult RayCastQueryFromStartOnTheLeft(WorkingMemory* workingMemory, const Vec2f& normalizedDir2d, RayCastQuery<NavTagTraversePredicate>& rayCastQuery,
		RayCastQueryResult& queryOnLeftResult, KyFloat32& borderDistOnLeft);


public:
	// ---------------------------------- Public Data Members ----------------------------------

	NavTagTraversePredicate m_predicate;
};

}

#include "gwnavruntime/queries/diskcangoquery.inl"


#endif //Navigation_DiskCanGoQuery_H


