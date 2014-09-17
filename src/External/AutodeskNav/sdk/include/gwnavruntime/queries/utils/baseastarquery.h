/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_AstarQueryImpl_H
#define Navigation_AstarQueryImpl_H

#include "gwnavruntime/queries/ipathfinderquery.h"
#include "gwnavruntime/navmesh/identifiers/navtriangleptr.h"
#include "gwnavruntime/querysystem/workingmemcontainers/navdatachangeindexingrid.h"
#include "gwnavruntime/queries/utils/pathrefinercontext.h"
#include "gwnavruntime/queries/utils/pathclampercontext.h"

namespace Kaim
{

class Path;
class QueryUtils;
class AStarTraversalContext;

/// Enumerates the possible results of an AStarQuery.
enum AStarQueryResult
{
	ASTAR_NOT_INITIALIZED = QUERY_NOT_INITIALIZED, ///< Indicates the query has not yet been initialized. 
	ASTAR_NOT_PROCESSED   = QUERY_NOT_PROCESSED,   ///< Indicates the query has not yet been launched. 

	ASTAR_PROCESSING_TRAVERSAL,                    ///< Indicates that the astar traversal algorithm has been launched, but has not yet completed. 
	ASTAR_PROCESSING_TRAVERSAL_DONE,               ///< Indicates that the astar traversal algorithm is finished, and that the path found is ready to be refined.
	ASTAR_PROCESSING_REFINING_INIT,                ///< Indicates that the refining process is being initialized.
	ASTAR_PROCESSING_REFINING,                     ///< Indicates that the path found is being refined.
	ASTAR_PROCESSING_PATHCLAMPING_INIT,            ///< Indicates that the refiner is done, but the path has not yet been clamped to the navMesh. 
	ASTAR_PROCESSING_PATHCLAMPING,                 ///< Indicates that the refined path is being clamped to navMesh according to the altitude tolerance.
	ASTAR_PROCESSING_PATHBUILDING,                 ///< Indicates that the clamping algorithm has completed, but the path has not yet been built. 

	ASTAR_DONE_START_OUTSIDE,                      ///< Indicates that a NavMesh triangle could not be found for the starting point. 
	ASTAR_DONE_START_NAVTAG_FORBIDDEN,             ///< Indicates that the NavTag at the starting point is forbidden. 
	ASTAR_DONE_END_OUTSIDE,                        ///< Indicates that the destination point of the query is outside the NavMesh. 
	ASTAR_DONE_END_NAVTAG_FORBIDDEN,               ///< Indicates that the NavTag at the destination point is forbidden. 
	ASTAR_DONE_PATH_MAX_COST_REACHED,              ///< Indicates that the traversal stops before finding a Path because the #m_pathMaxCost limit has been reached.
	ASTAR_DONE_PATH_NOT_FOUND,                     ///< Indicates that a path could not be found between the start and destination points.
	ASTAR_DONE_NAVDATA_CHANGED,                    ///< Indicates that the query has stopped and must be relaunched because of a change in the NavData.
	ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY,       ///< Indicates that insufficient working memory caused the query to stop. 
	ASTAR_DONE_COMPUTATION_ERROR,                  ///< Indicates that a computation error caused the query to stop.
	ASTAR_DONE_COMPUTATION_CANCELED,               ///< Indicates that the computation has been canceled.

	ASTAR_DONE_PATH_FOUND,                         ///< Indicates that a path has been found between the start and destination. 
};

/// Enumerates the possible way of dealing with startPos/EndPos slightly outside of the NavMesh.
enum AStarQueryHookingMode
{
	ASTAR_TRY_TO_HOOK_ON_NAVMESH       = 0, ///< If the startPos and/or the destPos is slightly outside the NavMesh,
	                                        ///<  we to try retrieve valid position inside and we pathfind with these points.
	ASTAR_DO_NOT_TRY_TO_HOOK_ON_NAVMESH     ///< If the startPos and/or the destPos is slightly outside the NavMesh,
	                                        ///<  we do not try retrieve valid position inside.

};

/// Base class for AstarQuery.
class BaseAStarQuery : public IPathFinderQuery
{
public:
	static BaseBlobHandler*  CreateStaticQueryBlobHandler();
	static  QueryType        GetStaticType() { return TypeAStar; }
	virtual QueryType        GetType() const { return TypeAStar; }
	virtual void             BuildQueryBlob(BaseBlobHandler* blobHandler);
	virtual void             InitFromQueryBlob(World* world, void* blob);


	// ---------------------------------- Main API functions ----------------------------------

	virtual void             GetPathFinderTextResult(String&) const;
	virtual PathFinderResult GetPathFinderResult()            const;
	virtual const Vec3f*     GetDestination()                 const;
	virtual BaseBlobHandler* CreateQueryBlobHandler();

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

public: // Internal
	// public because can be used by the Bot basic path finding helper functions
	void Initialize(Database* database, const Vec3f& startPos, const Vec3f& destPos);
	void SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr);

protected :
	BaseAStarQuery();
	virtual ~BaseAStarQuery() {}

	void SetDestTrianglePtr(const NavTrianglePtr& destTrianglePtr);
	void SetPathMaxCost(const KyFloat32 pathMaxCost);
	void SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange);
	void SetHookingMode(AStarQueryHookingMode aStarQueryHookingMode);
	void SetHorizontalHookingMaxDist(KyFloat32 horizontalHookingMaxDist);
	void SetResult(AStarQueryResult result);

	void ComputeRefinerNodeCost(PathRefinerContext* refinerContext, RefinerNode* currentNode);
	KyResult UpdateNodeInBinaryHeap(PathRefinerContext* refinerContext, RefinerNodeIndex nodeIdx, RefinerNode* currentNode);
	void DisplayListRefining(PathRefinerContext* refinerContext);
	KyResult BuildBinaryHeap(QueryUtils& queryUtils);


protected :
	/// The NavMesh triangle that corresponds to #m_startPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query.
	NavTrianglePtr m_startTrianglePtr;

	/// The NavMesh triangle that corresponds to #m_destPos3f. If a valid NavTrianglePtr is specified, this value will be used.
	/// If no value is specified, it will be automatically computed during the query.
	NavTrianglePtr m_destTrianglePtr;

	Vec3f m_destPos3f;        ///< The destination point for the path request. 
	Vec3f m_startInsidePos3f; ///< if #m_startPos3f is slightly outside and we hook on the NavMesh, it is the computed inside pos, else it is equal to #m_startPos3f.
	Vec3f m_destInsidePos3f;  ///< if #m_destPos3f is slightly outside and we hook on the NavMesh, it is the computed inside pos, else it is equal to #m_destPos3f.

	/// The altitude range around the position used when determining #m_startTrianglePtr and #m_destTrianglePtr from #m_startPos3f and #m_destPos3f. 
	PositionSpatializationRange m_positionSpatializationRange;

	/// The max distance from #m_startPos3f along the X axis and along the Y axis that will be searched for NavMesh if #m_startPos3f is outside the NavMesh
	///  and we have to try to hook on the NavMesh
	/// Basically, we will search for triangles in an axis aligned bounding bound around #m_startPos3f with an half extent of #m_horizontalHookDitance on x and y.
	/// Default value is 1.5f meters.
	KyFloat32 m_horizontalHookingMaxDist;
	KyFloat32 m_pathMaxCost; ///< The cost limit to prevent from flooding the AStar. The propagation won't visit nodes if their cost are greater than #m_pathMaxCost. Default value is 500.f meters

	AStarQueryResult m_result;           ///< Updated during processing to indicate the result of the PathFinderQueryResult query.
	AStarQueryHookingMode m_hookingMode; ///< Tells if we have to try to hook on the NavMesh if startPos/EndPos are slightly outside of the NavMesh.

	/// The number of nodes that will be processed each frame during the traversal part of the AStar.
	KyUInt32 m_numberOfVisitedNodePerFrame;

	/// The number of CanGo tests that will be processed each frame during the refiner part of the query.
	KyUInt32 m_numberOfCanGoTestInRefinerPerFrame;

	/// The number of intersections that will be processed each frame during the clamper part of the query.
	KyUInt32 m_numberOfIntersectionTestPerFrame;
};

KY_INLINE BaseBlobHandler*                   BaseAStarQuery::CreateQueryBlobHandler() { return CreateStaticQueryBlobHandler(); }
KY_INLINE IPathFinderQuery::PathFinderResult BaseAStarQuery::GetPathFinderResult() const
{
	switch(GetResult())
	{
	case ASTAR_NOT_INITIALIZED                   :
		return PathFinderNotInitialized;

	case ASTAR_NOT_PROCESSED                     :
		return PathFinderNotComputed;

	case ASTAR_PROCESSING_TRAVERSAL              :
	case ASTAR_PROCESSING_TRAVERSAL_DONE         :
	case ASTAR_PROCESSING_REFINING_INIT          :
	case ASTAR_PROCESSING_REFINING               :
	case ASTAR_PROCESSING_PATHCLAMPING_INIT      :
	case ASTAR_PROCESSING_PATHCLAMPING           :
	case ASTAR_PROCESSING_PATHBUILDING           :
		return PathFinderUnderProcess;

	case ASTAR_DONE_START_OUTSIDE                :
	case ASTAR_DONE_START_NAVTAG_FORBIDDEN       :
	case ASTAR_DONE_END_OUTSIDE                  :
	case ASTAR_DONE_END_NAVTAG_FORBIDDEN         :
	case ASTAR_DONE_PATH_MAX_COST_REACHED        :
	case ASTAR_DONE_PATH_NOT_FOUND               :
	case ASTAR_DONE_NAVDATA_CHANGED              :
	case ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY :
		return PathFinderFailure;

	case ASTAR_DONE_PATH_FOUND                   :
		return PathFinderSuccess;

	default:
		return PathFinderNotInitialized;
	}
}

KY_INLINE void BaseAStarQuery::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	m_positionSpatializationRange = positionSpatializationRange;
}
KY_INLINE void BaseAStarQuery::SetHookingMode(AStarQueryHookingMode hookingMode)               { m_hookingMode              = hookingMode;              }
KY_INLINE void BaseAStarQuery::SetPathMaxCost(const KyFloat32 pathMaxCost)                     { m_pathMaxCost              = pathMaxCost;              }
KY_INLINE void BaseAStarQuery::SetHorizontalHookingMaxDist(KyFloat32 horizontalHookingMaxDist) { m_horizontalHookingMaxDist = horizontalHookingMaxDist; }
KY_INLINE void BaseAStarQuery::SetResult(AStarQueryResult result)                              { m_result                   = result;                   }
KY_INLINE void BaseAStarQuery::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr)     { m_startTrianglePtr         = startTrianglePtr;         }
KY_INLINE void BaseAStarQuery::SetDestTrianglePtr(const NavTrianglePtr& destTrianglePtr)       { m_destTrianglePtr          = destTrianglePtr;          }

KY_INLINE AStarQueryResult      BaseAStarQuery::GetResult()                   const { return m_result;                        }
KY_INLINE AStarQueryHookingMode BaseAStarQuery::GetHookingMode()              const { return m_hookingMode;                   }
KY_INLINE KyFloat32             BaseAStarQuery::GetHorizontalHookingMaxDist() const { return m_horizontalHookingMaxDist;      }
KY_INLINE KyFloat32             BaseAStarQuery::GetPathMaxCost()              const { return m_pathMaxCost;                   }
KY_INLINE const Vec3f&          BaseAStarQuery::GetStartPos()                 const { return IPathFinderQuery::GetStartPos(); }
KY_INLINE const Vec3f&          BaseAStarQuery::GetDestPos()                  const { return m_destPos3f;                     }
KY_INLINE const NavTrianglePtr& BaseAStarQuery::GetStartTrianglePtr()         const { return m_startTrianglePtr;              }
KY_INLINE const NavTrianglePtr& BaseAStarQuery::GetDestTrianglePtr()          const { return m_destTrianglePtr;               }
KY_INLINE Path*                 BaseAStarQuery::GetPath()                     const { return m_path;                          }
KY_INLINE const Vec3f*          BaseAStarQuery::GetDestination()              const { return &GetDestPos();                   }

KY_INLINE const PositionSpatializationRange& BaseAStarQuery::GetPositionSpatializationRange() const { return m_positionSpatializationRange; }

}

#endif //Navigation_AstarQuery_H

