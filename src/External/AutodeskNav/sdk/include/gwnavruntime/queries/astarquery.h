/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_AstarQuery_H
#define Navigation_AstarQuery_H

#include "gwnavruntime/queries/utils/baseastarquery.h"
#include "gwnavruntime/queries/utils/astartraversal.h"
#include "gwnavruntime/queries/utils/astartraversalcontext.h"
#include "gwnavruntime/queries/raycastquery.h"
#include "gwnavruntime/queries/raycangoquery.h"
#include "gwnavruntime/queries/insideposfromoutsideposquery.h"
#include "gwnavruntime/database/database.h"
#include "gwnavruntime/path/path.h"


namespace Kaim
{

class QueryUtils;

/// Runs an A-star algorithm to find a Path through the navigation data (NavMesh &
/// NavGraph) from a starting point to a destination, refines the path found, and
/// fills an object of type Path.
///
/// The template argument should be a class that have these three functions : \code
/// bool      CanTraverse(const NavTag& navTag);
/// KyFloat32 GetGraphEdgeCost(onst Vec3f& edgeStart, const Vec3f& edgeEnd, const NavTag& navTag);
/// KyFloat32 GetHeuristicFromDistanceToDest(KyFloat32 distance3DToDestination); \endcode
/// The first function says which NavTag should be considered as navigable or not by the traversal. Typically it is
/// similar to the NavTagTraversePredicate template argument of a RayCanGoQuery or others queries.
/// The second function is used to get the cost of a NavGraph edge from its start and end position and its associated NavTag.
/// The third function can be used to modify the default Heuristic function based on euclidean distance.
/// Note that the template argument is stored in value within this class.
template<class AStarCustomizer = AStarCustomizer_Default>
class AStarQuery : public BaseAStarQuery
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------
	static QueryType        GetStaticType()                { return BaseAStarQuery::GetStaticType();                }
	static BaseBlobHandler* CreateStaticQueryBlobHandler() { return BaseAStarQuery::CreateStaticQueryBlobHandler(); }

	AStarQuery();
	AStarQuery(const AStarCustomizer& customizer);
	virtual ~AStarQuery();

	// ---------------------------------- Functions to set up the query ----------------------------------

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessors.
	/// \param database    A pointer to the Database on which the query must be performed.
	/// \param startPos    Sets the value of #m_startPos3f.
	/// \param destPos     Sets the value of #m_destPos3f. 
	void Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos);

	/// The NavMesh triangle that corresponds to #m_startPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query. 
	void SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr);

	/// The NavMesh triangle that corresponds to #m_destPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query.  
	void SetDestTrianglePtr(const NavTrianglePtr& destTrianglePtr);

	/// Write accessor for #m_hookingMode.
	/// Call this method after Initialize, during which #m_hookingMode is set to its default value.
	void SetHookingMode(AStarQueryHookingMode aStarQueryHookingMode);

	/// Write accessor for #m_horizontalHookingMaxDist.
	/// Call this method after Initialize(), during which #m_horizontalHookingMaxDist is set to its default value.
	void SetHorizontalHookingMaxDist(KyFloat32 horizontalHookingMaxDist);

	/// Write accessor for #m_pathMaxCost.
	/// Call this method after Initialize(), during which #m_pathMaxCost is set to its default value.
	void SetPathMaxCost(KyFloat32 pathMaxCost);

	/// Write accessor for #m_positionSpatializationRange.
	/// Call this method after Initialize(), during which #m_positionSpatializationRange is set to its default value.
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);

	/// Write accessor for #m_numberOfVisitedNodePerFrame.
	/// Call this method after Initialize(), during which #m_numberOfVisitedNodePerFrame is set to its default value.
	void SetNumberOfProcessedNodePerFrame(KyUInt32 numberOfProcessedNodePerFrame);

	// ---------------------------------- Query framework functions ----------------------------------

	/// Performs a single iteration of the PathFinder.
	/// \copydoc IQuery::Advance()
	virtual void Advance(WorkingMemory* workingMemory);

	virtual void ReleaseWorkingMemoryOnCancelDuringProcess(WorkingMemory* workingMemory = KY_NULL);

	// ---------------------------------- Accessors ----------------------------------
	AStarQueryResult      GetResult()                        const;
	AStarQueryHookingMode GetHookingMode()                   const;
	KyFloat32             GetHorizontalHookingMaxDist()      const;
	KyFloat32             GetPathMaxCost()                   const;
	const Vec3f&          GetStartPos()                      const;
	const Vec3f&          GetDestPos()                       const;
	const NavTrianglePtr& GetStartTrianglePtr()              const;
	const NavTrianglePtr& GetDestTrianglePtr()               const;
	Path*                 GetPath()                          const;
	KyUInt32              GetNumberOfProcessedNodePerFrame() const;

	const PositionSpatializationRange& GetPositionSpatializationRange() const;

public: //Internal
	bool HasNavDataChanged();

protected:
	void SetFinish(WorkingMemory* workingMemory);
	KyResult ComputeStartTriangle(QueryUtils& queryUtils);
	KyResult ComputeDestTriangle(QueryUtils& queryUtils);
	KyResult TryToHookOnNavMesh(QueryUtils& queryUtils, const Vec3f& inputOutsidePos, Vec3f& outputInsidePos, NavTrianglePtr& outputNavTrianglePtr);
	KyResult InitRefinerContextFromAstarTraversalContext(WorkingMemory* workingMemory);
	KyResult RefineOneNode(PathRefinerContext* refinerContext, QueryUtils& queryUtils, KyUInt32& cangoTestDone);
	KyResult CheckNavDataChange(WorkingMemory* workingMemory);

protected:
	AStarTraversal<AStarCustomizer> m_astarTraversal;
};

}

#include "gwnavruntime/queries/astarquery.inl"

#endif //Navigation_AstarQuery_H

