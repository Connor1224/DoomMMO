/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: MUBI - secondary contact: LAPA

#ifndef Navigation_VelocityObstacle_H
#define Navigation_VelocityObstacle_H

#include "gwnavruntime/math/vec2f.h"
#include "gwnavruntime/kernel/SF_Math.h"
#include "gwnavruntime/pathfollower/velocityobstaclecollider.h"

namespace Kaim
{

class VelocityObstacleCollider;

/// A VelocityObstacle is a cone built relatively from a particle A to a particle B.
/// This obstacle represent the space of velocities that the particle A should not use in order
/// to avoid a collision with the particle B.
/// We define the cone by its apex position and an inscribe circle
class VelocityObstacle
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	/// Enumerates the possible results returned by VelocityObstacle::IsVelocityCompatible.
	enum VelocityCompatibilityResult
	{
		/// The velocity is compatible with the VelocityObstacle.
		Outside,

		/// Applying the velocity to the Bot will lead to a collision with
		/// object related to this VelocityObstacle.
		Inside,

		/// The object related to this VelocityObstacle and the Bot are already
		/// in collision. In such case, you can either let them in collision and
		/// ignore the VelocityObstacle or take particular decision like a
		/// repulsion reflex.
		AlreadyInCollision
	};


	// ---------------------------------- Public Member Functions ----------------------------------

	VelocityObstacle()
		: m_apex     (0.0f, 0.0f)
		, m_circleCenter     (0.0f, 0.0f)
		, m_circleRadius((KyFloat32)(KY_MATH_PI_4))
	{}

	/// Reset and compute the VelocityObstacle for a given obstacle respective to a reference root obstacle
	void Reset(const VelocityObstacleCollider& rootCollider, const VelocityObstacleCollider& collider);

	/// Test a sample velocity against the VelocityObstacle
	VelocityCompatibilityResult IsVelocityCompatible(const Vec2f& sampleVelocity) const;


	// ---------------------------------- Public Data Members ----------------------------------

	Vec2f m_apex;
	Vec2f m_circleCenter;
	KyFloat32 m_circleRadius;
};

inline void SwapEndianness(Endianness::Target e, VelocityObstacle& self)
{
	SwapEndianness(e, self.m_apex);
	SwapEndianness(e, self.m_circleCenter);
	SwapEndianness(e, self.m_circleRadius);
}

} // namespace Kaim

#endif // Navigation_VelocityObstacle_H
