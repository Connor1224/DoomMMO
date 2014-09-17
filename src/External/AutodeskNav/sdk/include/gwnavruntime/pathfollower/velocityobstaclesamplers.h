/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: MUBI - secondary contact: LAPA

#ifndef Navigation_VelocityObstacleSamplers_H
#define Navigation_VelocityObstacleSamplers_H

#include "gwnavruntime/math/vec2f.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/kernel/SF_Math.h"
#include "gwnavruntime/pathfollower/velocityobstacleworkspace.h"
#include "gwnavruntime/spatialization/spatializedpoint.h"
#include "gwnavruntime/queries/raycangoquery.h"
#include "gwnavruntime/queries/raycastquery.h"

namespace Kaim
{

/// This class samples velocity candidates for avoidance and checks validity against
/// all the VelocityObstacles that are retained in the VelocityObstacleWorkspace.
/// The sampling acts on candidate speed (i.e. the velocity length).
class ScaleVelocityToSafeSpeed
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	ScaleVelocityToSafeSpeed()
		: m_maximalDesiredVelocity(0.0f, 0.0f)
		, m_sampleCount(4)
	{}

	/// Compute and checks velocity candidates sampled along #m_maximalDesiredVelocity,
	/// in decreasing speed order.
	/// It returns true at the first valid candidate it founds. Returns false when
	/// none of the sampled speeds are compatible with the VelocityObstacles.
	/// If doesn't check null speed that has to be treated as particular case.
	bool FindSafeVelocity(const VelocityObstacleWorkspace& workspace, Vec2f& output);


	// ---------------------------------- Public Data Members ----------------------------------

	Vec2f m_maximalDesiredVelocity; ///< The desired velocity.
	KyUInt32 m_sampleCount;
};


/// This class samples velocity candidates for avoidance and checks validity against
/// all the VelocityObstacles that are retained in the VelocityObstacleWorkspace.
/// The sampling acts on candidate direction.
template<class NavTagTraversePredicate>
class OrientVelocityToClosestSafeDirection
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	OrientVelocityToClosestSafeDirection()
		: m_predicate() , m_originalVelocity(0.0f, 0.0f) , m_angleSpan((KyFloat32)(KY_MATH_PI * 2.0f)) , m_sampleCount(72) 
	{}

	OrientVelocityToClosestSafeDirection(const NavTagTraversePredicate& predicate)
		: m_predicate(predicate)
		, m_originalVelocity(0.0f, 0.0f)
		, m_angleSpan       ((KyFloat32)(KY_MATH_PI * 2.0f))
		, m_sampleCount     (72)
	{}

	/// Compute and checks velocity candidates sampled on a circle arc centered
	/// on #m_originalVelocity and restricted to #m_angleSpan overall opening
	/// angle. The candidates are generated and validated starting from #m_originalVelocity
	/// and then alternatively on right then on left with increasing angle from cone center.
	/// It returns true at the first valid candidate it founds. Returns false when
	/// none of the sampled speeds are compatible with the VelocityObstacles.
	/// If doesn't check null speed that has to be treated as particular case.
	bool FindSafeVelocity(bool enableBackMove, const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition, const NavTrianglePtr& botTrianglePtr, const Vec2f& previousVelocity, const Vec2f& desiredVelocity, Vec2f& result);

protected:
	bool TestDirection          (const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition, const NavTrianglePtr& botTrianglePtr, const Vec2f& testedDirection, KyFloat32 originalSpeed);
	bool FindSafeVelocityInRange(const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition, const NavTrianglePtr& botTrianglePtr, const Vec2f& from, const Vec2f& to, KyFloat32 originalSpeed, Vec2f& result);


public:
	// ---------------------------------- Public Data Members ----------------------------------

	NavTagTraversePredicate m_predicate; ///< The predicate use to validate the velocity candidate against NavData.
	Vec2f m_originalVelocity; ///< The central axis of candidate cone.
	KyFloat32 m_angleSpan; ///< The complete candidate cone open angle, expressed in radians.
	KyFloat32 m_sampleCount;
};


} // namespace Kaim

#include "gwnavruntime/pathfollower/velocityobstaclesamplers.inl"

#endif // Navigation_VelocityObstacleSamplers_H
