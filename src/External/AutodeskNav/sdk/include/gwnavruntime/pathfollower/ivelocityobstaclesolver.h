/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: MUBI - secondary contact: LAPA
#ifndef Navigation_IVelocityObstacleSolver_H
#define Navigation_IVelocityObstacleSolver_H

#include "gwnavruntime/pathfollower/iavoidancecomputer.h"

namespace Kaim
{
class AvoidanceComputerConfig;
class SpatializedPoint;
class VelocityObstacleWorkspace;
class Database;


/// Configuration class for VelocityObstacle solvers.
class VelocityObstacleSolverConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	VelocityObstacleSolverConfig(const AvoidanceComputerConfig* avoidanceComputerConfig,
	                             const SpatializedPoint* colliderSpatializedPoint,
	                             VelocityObstacleWorkspace* workspace,
	                             Database* database,
	                             KyFloat32 simulationTimeInSeconds)
		: m_avoidanceComputerConfig(avoidanceComputerConfig)
		, m_colliderSpatializedPoint(colliderSpatializedPoint)
		, m_workspace(workspace)
		, m_database(database)
		, m_deltaTime(simulationTimeInSeconds)
	{
	}
	

	// ---------------------------------- Public Data Members ----------------------------------

	const AvoidanceComputerConfig* m_avoidanceComputerConfig;
	const SpatializedPoint*        m_colliderSpatializedPoint;
	VelocityObstacleWorkspace*     m_workspace;
	Database*                      m_database;
	KyFloat32                      m_deltaTime;
};


/// Interface class for VelocityObstacle solvers.
class IVelocityObstacleSolver: public RefCountBase<IVelocityObstacleSolver, MemStat_PathFollowing>
{
	KY_CLASS_WITHOUT_COPY(IVelocityObstacleSolver)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	IVelocityObstacleSolver() {}
	virtual ~IVelocityObstacleSolver() {}

	virtual IAvoidanceComputer::AvoidanceResult Solve(VelocityObstacleSolverConfig& config, const Vec3f& previousVelocity, const Vec3f& desiredVelocity, Vec3f& outputVelocity) = 0;
};

} // namespace Kaim

#endif // Navigation_IVelocityObstacleSolver_H
