/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: MUBI - secondary contact: LAPA

#ifndef Navigation_VelocityObstacleCollider_H
#define Navigation_VelocityObstacleCollider_H

#include "gwnavruntime/math/vec2f.h"

namespace Kaim
{

/// A Collider is a 2D circular particle with a linear velocity.
/// It is the most basic unit one can use to compute the VelocityObstacles.
class VelocityObstacleCollider
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	VelocityObstacleCollider()
		: m_position    (Vec2f(0.0f, 0.0f))
		, m_velocity    (Vec2f(0.0f, 0.0f))
		, m_radius      (0.5f)
	{}

	KyFloat32 ComputeMinimumTimeToCollision(const VelocityObstacleCollider& collider) const;


	// ---------------------------------- Public Data Members ----------------------------------

	Vec2f     m_position;
	Vec2f     m_velocity;
	KyFloat32 m_radius;
};

inline void SwapEndianness(Endianness::Target e, VelocityObstacleCollider& self)
{
	SwapEndianness(e, self.m_position);
	SwapEndianness(e, self.m_velocity);
	SwapEndianness(e, self.m_radius);
}

} // namespace Kaim

#endif // Navigation_VelocityObstacleCollider_H
