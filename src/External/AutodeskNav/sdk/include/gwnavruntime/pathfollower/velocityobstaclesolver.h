/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: MUBI - secondary contact: LAPA
#ifndef Navigation_VelocityObstacleSolver_H
#define Navigation_VelocityObstacleSolver_H

#include "gwnavruntime/pathfollower/velocityobstaclesamplers.h"
#include "gwnavruntime/pathfollower/avoidancecomputerconfig.h"
#include "gwnavruntime/pathfollower/iavoidancecomputer.h"
#include "gwnavruntime/pathfollower/ivelocityobstaclesolver.h"


namespace Kaim
{


/// VelocityObstacleSever computes a velocity compatible with all the colliders in
/// VelocityObstacleWorkspace and the NavData accordingly to NavTagTraversePredicate.
template<class NavTagTraversePredicate>
class VelocityObstacleSolver : public IVelocityObstacleSolver
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	VelocityObstacleSolver()
		: m_predicate()
		, m_scaleVelocityToSafeSpeed()
		, m_orientVelocityToClosestSafeDirection()
	{}

	VelocityObstacleSolver(const NavTagTraversePredicate& predicate)
		: m_predicate(predicate)
		, m_scaleVelocityToSafeSpeed()
		, m_orientVelocityToClosestSafeDirection(predicate)
	{}

	/// Compute the more appropriate velocity. It stops at the first compatible
	/// one in this order:
	/// -# directly check desiredVelocity
	/// -# try reducing speed in desiredVelocity direction
	/// -# search solutions in other directions.
	/// If no solution is found, outputVelocity is set to null vector.
	virtual IAvoidanceComputer::AvoidanceResult Solve(VelocityObstacleSolverConfig& config,
		const Vec3f& previousVelocity, const Vec3f& desiredVelocity, Vec3f& outputVelocity);


	// ---------------------------------- Public Data Members ----------------------------------

	/// The predicate used for velocity candidate validation against NavData.
	NavTagTraversePredicate m_predicate;

	/// The velocity sampler used to compute and check several velocities in the
	/// same direction but with various speed values.
	ScaleVelocityToSafeSpeed m_scaleVelocityToSafeSpeed;

	/// The velocity sampler used to compute velocities in various directions.
	OrientVelocityToClosestSafeDirection<NavTagTraversePredicate> m_orientVelocityToClosestSafeDirection;
};

} // namespace Kaim

#include "gwnavruntime/pathfollower/velocityobstaclesolver.inl"

#endif // Navigation_VelocityObstacleSolver_H
