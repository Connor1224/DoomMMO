/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_FindFirstVisiblePositionOnPathQuery_H
#define Navigation_FindFirstVisiblePositionOnPathQuery_H

#include "gwnavruntime/queries/utils/basefindfirstvisiblepositiononpathquery.h"
#include "gwnavruntime/database/database.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmemarray.h"
#include "gwnavruntime/visualsystem/displaylist.h"


namespace Kaim
{

/// The FindFirstVisiblePositionOnPathQuery class searches for a PositionOnPath that
/// can be reached from a provided point (#m_visibilityStartPos3f).
/// It samples the path starting from #m_startPositionOnPath up to #m_endPositionOnPath,
/// and stops at the first visible PositionOnPath it reaches.
template<class NavTagTraversePredicate>
class FindFirstVisiblePositionOnPathQuery : public BaseFindFirstVisiblePositionOnPathQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	FindFirstVisiblePositionOnPathQuery();
	FindFirstVisiblePositionOnPathQuery(const NavTagTraversePredicate& predicate);
	virtual ~FindFirstVisiblePositionOnPathQuery() {}

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	/// \param database                A pointer to the Database on which the query must be performed. Sets the value of m_database
	/// \param startPositionOnPath     Sets the value of #m_startPositionOnPath.
	/// \param endPositionOnPath       Sets the value of #m_endPositionOnPath.
	/// \param visibilityStartPoint    Sets the value of #m_visibilityStartPos3f.
	void Initialize(Database* database, const PositionOnPath& startPositionOnPath, const PositionOnPath& endPositionOnPath, const Vec3f& visibilityStartPoint);

	/// Set The NavMesh triangle that corresponds to #m_visibilityStartPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query.
	/// Call this method after Initialize(), during which #m_visibilityStartTrianglePtr is set to its default value. 
	void SetVisibilityStartTrianglePtr(const NavTrianglePtr& visibilityStartTrianglePtr);

	void SetSamplingDistance(KyFloat32 samplingDistance);

	// ---------------------------------- Query framework functions ----------------------------------

	virtual void Advance(WorkingMemory* workingMemory);

	/// Performs the query.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                          the #m_database internal WorkingMemory is used.
	void PerformQuery(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------

	FindFirstVisiblePositionOnPathQueryResult GetResult()                      const;
	const PositionOnPath&                     GetStartPositionOnPath()         const;
	const PositionOnPath&                     GetEndPositionOnPath()           const;
	const Vec3f&                              GetVisibilityStartPoint()        const;
	const NavTrianglePtr&                     GetVisibilityStartTrianglePtr()  const;
	KyFloat32                                 GetSamplingDistance()            const;
	const PositionOnPath&                     GetFirstVisiblePositionOnPath()  const;

private:
	CandidateValidationResult ValidateCandidate(
		RayCanGoQuery<NavTagTraversePredicate>& rayCanGoQuery, const PositionOnPath& candidate, WorkingMemory* workingMemory);

public:
	NavTagTraversePredicate m_predicate;
	ScopedDisplayList* m_displayList;
};

}

#include "gwnavruntime/queries/findfirstvisiblepositiononpathquery.inl"


#endif // Navigation_FindFirstVisiblePositionOnPathQuery_H

