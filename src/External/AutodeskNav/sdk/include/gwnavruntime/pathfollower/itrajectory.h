/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_ITrajectory_H
#define Navigation_ITrajectory_H

#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/pathfollower/iavoidancecomputer.h"

namespace Kaim
{

class Bot;
class VisualDebugServer;


/// This interface class defines at the same time the trajectory data structure
/// and its computation method.
/// We provide a default simple trajectory (see Trajectory class).
/// If you need more complex trajectories (e.g. based on splines), you have to
/// implement your own ITrajectory. In that case, don't forget to compute
/// ITrajectory::m_velocity that is used for Bot position prediction.
class ITrajectory: public RefCountBase<ITrajectory, MemStat_PathFollowing>
{
	KY_CLASS_WITHOUT_COPY(ITrajectory)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	ITrajectory() : m_avoidanceResult(IAvoidanceComputer::NoAvoidance) {}
	virtual ~ITrajectory() {}

	/// Computes trajectory values for the Bot so that it reaches its current target on path.
	virtual void Compute(Bot* bot, KyFloat32 simulationTimeInSeconds) = 0;

	virtual void SendVisualDebugBlob(VisualDebugServer&, Bot*) {}

	KY_INLINE const Vec3f& GetVelocity() const { return m_velocity; }
	KY_INLINE const IAvoidanceComputer::AvoidanceResult& GetAvoidanceResult() const { return m_avoidanceResult; }

public: // internal
	Vec3f m_velocity;
	IAvoidanceComputer::AvoidanceResult m_avoidanceResult;
};

} // namespace Kaim

#endif // Navigation_ITrajectory_H
