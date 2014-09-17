/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_Trajectory_H
#define Navigation_Trajectory_H

#include "gwnavruntime/pathfollower/itrajectory.h"
#include "gwnavruntime/pathfollower/trajectoryblob.h"
#include "gwnavruntime/pathfollower/iavoidancecomputer.h"
#include "gwnavruntime/path/patheventlist.h"
#include "gwnavruntime/visualdebug/visualdebugserver.h"
#include "gwnavruntime/world/bot.h"


namespace Kaim
{

class SpatializedCylinder;
class Database;


/// This class computes minimal trajectory made of just one velocity. It can be configured to:
///  - either always goes straight towards target on path at maxSpeed (when Avoidance is disabled)
///  - or delegate to an IAvoidanceComputer the computation of a velocity compatible with
///    surrounding moving obstacles and bots (when Avoidance is enabled).
class Trajectory : public ITrajectory
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	Trajectory()
		: m_avoidanceComputer(KY_NULL)
		, m_forceTargetOnTargetOnPath(true)
		, m_target(0.0f, 0.0f, 0.0f)
	{}

	virtual void Compute(Bot* bot, KyFloat32 simulationTimeInSeconds);

	void SetAvoidanceComputer(Ptr<IAvoidanceComputer> avoidanceComputer) { m_avoidanceComputer = avoidanceComputer; }

	const Vec3f& GetTrajectoryTarget() const { return m_target; }

	virtual void UpdateTarget(Bot* bot); //< Called by the compute method. Refresh the trajectory target used to compute the velocity.
	void ForceTargetToTargetOnPath();
	void ForceTargetToPosition(const Vec3f& forcedTarget);

public: // internal
	virtual void SendVisualDebugBlob(VisualDebugServer& visualDebugServer, Bot* bot);

protected:
	void ComputeStraightTrajectory(Bot* bot, KyFloat32 simulationTimeInSeconds);
	void ComputeAvoidanceTrajectory(Bot* bot, KyFloat32 simulationTimeInSeconds);


protected:
	Ptr<IAvoidanceComputer> m_avoidanceComputer;
	bool                    m_forceTargetOnTargetOnPath;
	Kaim::Vec3f             m_target;
	
};


KY_INLINE void Trajectory::ForceTargetToTargetOnPath() { m_forceTargetOnTargetOnPath = true; }
KY_INLINE void Trajectory::ForceTargetToPosition(const Vec3f& forcedTarget)
{ 
	m_forceTargetOnTargetOnPath = false; 
	m_target = forcedTarget;
}

} // namespace Kaim

#endif // Navigation_Trajectory_H
