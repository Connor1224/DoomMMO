/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: MUBI - secondary contact: LAPA
#ifndef Navigation_IAvoidanceComputer_H
#define Navigation_IAvoidanceComputer_H

#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

namespace Kaim
{

class Bot;


/// This interface class defines the way avoidance is computed.
/// We provide a default AvoidanceComputer (based on VelocityObstacles).
/// You can implement your own IAvoidanceComputer to be used either in
/// our Trajectory class or in your own ITrajectory implementation class.
class IAvoidanceComputer: public RefCountBase<IAvoidanceComputer, MemStat_PathFollowing>
{
	KY_CLASS_WITHOUT_COPY(IAvoidanceComputer)

public:
	/// Enumerates the possible results of a call to IAvoidanceComputer::Compute().
	enum AvoidanceResult
	{
		NoAvoidance = 0, ///< The trajectory goes in target direction at the desired speed.
		SlowDown,        ///< The trajectory goes in target direction but at lower speed than the desired one.
		Accelerate,      ///< The trajectory goes in target direction but at higher speed than the desired one.
		Turn,            ///< The trajectory deviates from target direction.
		Stop,            ///< The avoidance computer decided that stopoing the Bot is the best solution.
		SolutionNotFound ///< The avoidance computer was not able to find any solution.
	};


	// ---------------------------------- Public Member Functions ----------------------------------

	IAvoidanceComputer() {}
	virtual ~IAvoidanceComputer() {}

	// ---------------------------------- Pure Virtual Functions ----------------------------------

	virtual AvoidanceResult Compute(Bot& bot, KyFloat32 simulationTimeInSeconds, const Vec3f& target, Vec3f& avoidanceVelocity) = 0;
};

} // namespace Kaim

#endif // Navigation_IAvoidanceComputer_H
