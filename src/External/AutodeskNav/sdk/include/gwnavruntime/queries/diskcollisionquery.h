/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_DiskCollisionQuery_H
#define Navigation_DiskCollisionQuery_H

#include "gwnavruntime/queries/utils/basediskcollisionquery.h"
#include "gwnavruntime/queries/utils/diskintersector.h"
#include "gwnavruntime/queries/utils/breadthfirstsearchedgecollisionvisitor.h"
#include "gwnavruntime/queries/utils/breadthfirstsearchtraversal.h"

namespace Kaim
{
/// The DiskCollisionQuery class attempts to place the center of a disk with a specified radius (#m_radius) at a
/// specified position (#m_centerPos3f), and tests whether the entire disk fits within the borders of the NavMesh
/// without intersecting any forbidden NavTag (NavMesh borders).
template<class NavTagTraversePredicate>
class DiskCollisionQuery : public BaseDiskCollisionQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	DiskCollisionQuery();
	DiskCollisionQuery(const NavTagTraversePredicate& predicate);
	virtual ~DiskCollisionQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database    A pointer to the Database on which the query must be performed. Sets the value of m_database.
	/// \param center      Sets the value of #m_centerPos3f. 
	/// \param radius      Sets the value of #m_radius. 
	void Initialize(Database* database, const Vec3f& center, KyFloat32 radius);

	/// The NavMesh triangle that corresponds to #m_centerPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query. 
	/// Call this method after Initialize(), during which #m_centerTrianglePtr is set to its default value. 
	void SetCenterTrianglePtr(const NavTrianglePtr& centerTrianglePtr);

	/// Write accessor for #m_positionSpatializationRange.
	/// Call this method after Initialize(), during which #m_positionSpatializationRange is set to its default value.
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

	DiskCollisionQueryResult GetResult()             const;
	const Vec3f&             GetCenterPos()          const;
	const NavTrianglePtr&    GetCenterTrianglePtr()  const;
	KyFloat32                GetRadius()             const;
	DynamicOutputMode        GetDynamicOutputMode()  const;
	QueryDynamicOutput*      GetQueryDynamicOutput() const;
	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetCenterIntegerPos(const WorldIntegerPos& centerIntegerPos);
	const WorldIntegerPos& GetCenterIntegerPos() const;

public:
	NavTagTraversePredicate m_predicate;
};

}

#include "gwnavruntime/queries/diskcollisionquery.inl"


#endif //Navigation_DiskCollisionQuery_H

