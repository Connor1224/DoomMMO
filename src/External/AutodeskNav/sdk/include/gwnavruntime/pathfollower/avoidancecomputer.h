/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_AvoidanceComputer_H
#define Navigation_AvoidanceComputer_H

#include "gwnavruntime/pathfollower/iavoidancecomputer.h"
#include "gwnavruntime/pathfollower/iavoidancefilter.h"
#include "gwnavruntime/pathfollower/velocityobstacleworkspace.h"
#include "gwnavruntime/queries/spatializedpointcollectorinaabbquery.h"
#include "gwnavruntime/pathfollower/velocityobstaclesolver.h"
#include "gwnavruntime/pathfollower/velocityobstacleworkspaceblob.h"
#include "gwnavruntime/world/world.h"
#include "gwnavruntime/world/bot.h"

namespace Kaim
{
class SpatializedCylinder;
class SpatializedPoint;
class AvoidanceComputer;


/// Class used to filter potential colliders in AvoidanceComputer at SpatializedPoint
/// collection step.
class AvoidanceComputerCollectorFilter
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	AvoidanceComputerCollectorFilter()
		: m_bot(KY_NULL)
		, m_avoidanceComputer(KY_NULL)
	{}

	bool ShouldCollectSpatializedPoint(SpatializedPoint* spatializedPoint);


	// ---------------------------------- Public Data Members ----------------------------------

	Bot* m_bot;
	AvoidanceComputer* m_avoidanceComputer;
};


/// This class uses VelocityObstacles to compute collision-free trajectories.
/// It is used by the Trajectory class to delegate avoidance trajectory
/// computation. You can reuse it in your own ITrajectory implementation class.
class AvoidanceComputer : public IAvoidanceComputer
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)
	KY_CLASS_WITHOUT_COPY(AvoidanceComputer)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	AvoidanceComputer();

	virtual AvoidanceResult Compute(Bot& bot, KyFloat32 simulationTimeInSeconds, const Vec3f& target, Vec3f& avoidanceVelocity);

	void SetVelocityObstacleSolver(Ptr<IVelocityObstacleSolver> velocityObstacleSolver) { m_velocityObstacleSolver = velocityObstacleSolver; }
	void SetAvoidanceFilter       (Ptr<IAvoidanceFilter> avoidanceFilter)               { m_avoidanceFilter = avoidanceFilter; }

private:
	friend class AvoidanceComputerCollectorFilter;

	void CollectSpatializedPoints(Bot& bot);
	void SetupVOWorkspace(const Bot& bot, const Vec3f& target);
	void FillVOWorkspace(const Bot& bot);

	IAvoidanceComputer::AvoidanceResult SolveVOWorkspace(const Bot& bot, KyFloat32 simulationTimeInSeconds, const Vec3f& target, Vec3f& avoidanceVelocity);

	bool DoesCollectionQueryNeedToBeUpdated(const Bot& bot);
	const SpatializedCylinder* GetSpatializedCylinderFromSpatializedPoint(SpatializedPoint* spatializedPoint);
	VelocityObstacleWorkspaceAddColliderResult AddColliderToVOWorkspace(VelocityObstacleWorkspace& velocityObstacleWorkspace, const SpatializedCylinder& colliderCylinder, const KyFloat32 databaseRadiusPrecision);

	Ptr<IVelocityObstacleSolver> m_velocityObstacleSolver; // If let to KY_NULL, AvoidanceComputer will instantiate a VelocityObstacleSolver with the world default NavTag Predicate
	SpatializedPointCollectorInAABBQuery<AvoidanceComputerCollectorFilter> m_collectQuery; // Put this in a Ptr<> if you want to push it in a queryQueue
	Ptr<IAvoidanceFilter> m_avoidanceFilter;
};


} // namespace Kaim

#endif // Navigation_AvoidanceComputer_H
