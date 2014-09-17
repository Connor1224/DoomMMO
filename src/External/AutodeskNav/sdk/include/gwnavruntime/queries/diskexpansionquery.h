/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_DiskExpansionQuery_H
#define Navigation_DiskExpansionQuery_H

#include "gwnavruntime/queries/utils/basediskexpansionquery.h"
#include "gwnavruntime/queries/utils/diskintersector.h"
#include "gwnavruntime/queries/utils/bestfirstsearchedgecollisionvisitor.h"
#include "gwnavruntime/queries/utils/bestfirstsearchtraversal.h"

namespace Kaim
{

/// The DiskExpansionQuery class attempts to place the center of a disk at a specified position (#m_centerPos3f), and
/// expands the radius of the disk until the disk:
/// -    collides with a NavMesh border
/// -    intersects a forbidden NavTag (a triangle with a NavTag considered as non-traversable by the template argument)
/// -    reaches the maximum size (#m_radiusMax)
template<class NavTagTraversePredicate>
class DiskExpansionQuery : public BaseDiskExpansionQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	DiskExpansionQuery();
	DiskExpansionQuery(const NavTagTraversePredicate& predicate);
	~DiskExpansionQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database     A pointer to the Database on which the query must be performed. Sets the value of m_database.
	/// \param center       Sets the value of #m_centerPos3f.
	/// \param radiusMax    Sets the value of #m_radiusMax.
	void Initialize(Database* database, const Vec3f& center, KyFloat32 radiusMax);

	/// The NavMesh triangle that corresponds to #m_centerPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query. 
	/// Call this method after Initialize(), during which #m_centerTrianglePtr is set to its default value. 
	void SetCenterTrianglePtr(const NavTrianglePtr& centerTrianglePtr);

	/// Write accessor for #m_positionSpatializationRange.
	/// Call this method after Initialize(), during which #m_positionSpatializationRange is set to its default value.
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	/// Write accessor for #m_safetyDist.
	/// Call this method after Initialize(), during which #m_safetyDist is set to its default value. 
	void SetSafetyDist(KyFloat32 safetyDist);

	/// Write accessor for #m_dynamicOutputMode.
	/// Call this method after Initialize(), during which  #m_dynamicOutputMode is set to its default value. 
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

	DiskExpansionQueryResult GetResult()                  const;
	const Vec3f&             GetCenterPos()               const;
	const NavTrianglePtr&    GetCenterTrianglePtr()       const;
	KyFloat32                GetRadiusMax()               const;
	KyFloat32                GetResultRadius()            const;
	const Vec3f&             GetCollisionPos()            const;
	const NavHalfEdgePtr&    GetCollisionNavHalfEdgePtr() const;
	KyFloat32                GetSafetyDist()              const;
	DynamicOutputMode        GetDynamicOutputMode()       const;
	QueryDynamicOutput*      GetQueryDynamicOutput()      const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public : // internal
	void PerformQueryWithInputCoordPos(WorkingMemory* workingMemory);
	void SetCenterIntegerPos(const WorldIntegerPos& centerIntegerPos);
	const WorldIntegerPos& GetCenterIntegerPos() const;

public:
	NavTagTraversePredicate m_predicate;
};

}

#include "gwnavruntime/queries/diskexpansionquery.inl"



#endif //Navigation_DiskCollisionQuery_H

