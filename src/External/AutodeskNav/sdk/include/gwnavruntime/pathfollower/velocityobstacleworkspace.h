/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: MUBI - secondary contact: LAPA

#ifndef Navigation_VelocityObstacleWorkspace_H
#define Navigation_VelocityObstacleWorkspace_H

#include "gwnavruntime/pathfollower/velocityobstaclecollider.h"
#include "gwnavruntime/pathfollower/velocityobstacle.h"
#include "gwnavruntime/containers/kyarray.h"

namespace Kaim
{

// We want theses arrays to work on contiguous memory, never to release it
typedef KyArrayPOD<VelocityObstacleCollider, MemStat_PathFollowing, Kaim::ArrayConstPolicy<0, 16, true> > ColliderArray;
typedef KyArrayPOD<VelocityObstacle,         MemStat_PathFollowing, Kaim::ArrayConstPolicy<0, 16, true> > VelocityObstacleArray;


enum VelocityObstacleWorkspaceAddColliderResult
{
	VelocityObstacleWorkspace_ColliderAdded = 0, ///<
	VelocityObstacleWorkspace_ColliderIgnored ///<
};


/// VelocityObstacleWorkspace aggregates at once:
/// - a root collider, that represent the Bot for which a velocity as to be found
/// - the VelocityObstacle for nearby potential collider to be avoidance.
/// It is typically fed by an IAvoidanceComputer instance and then provided
/// to a IVelocityObstacleSolver to compute the more appropriate velocity.
class VelocityObstacleWorkspace
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_PathFollowing)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	VelocityObstacleWorkspace()
		: m_minimalTimeToCollision(0.5f)
		, m_doSendVisualDebugBlob(false)
	{}

	/// Reset the workspace and prepare it for a given root collider.
	void Reset(const VelocityObstacleCollider& rootCollider);

	/// Compute the time to collision and filters accordingly:
	/// - if bellow #m_minimalTimeToCollision: insert the collider in the workspace
	///   and return VelocityObstacleWorkspace_ColliderAdded
	/// - else: ignore the collider and return VelocityObstacleWorkspace_ColliderIgnored
	/// All colliders are however retained in a ColliderArray, accordingly to the
	/// filtering, an can be retrieved there afterwards.
	VelocityObstacleWorkspaceAddColliderResult AddCollider(const VelocityObstacleCollider& collider);

	/// Checks candidateVelocity against all VelocityObstacles in the workspace.
	bool IsVelocitySafe(const Vec2f& candidateVelocity) const;

	/// Simulate the next frame situation that should occur under these conditions:
	/// - the root collider moves accordingly to avoidanceVelocity
	/// - the other colliders keep their current velocities.
	/// This function changes the internals, you can't get original situation anymore.
	/// This function can be useful to validate one step beyond a hard situation
	/// in which the avoidance velocity found for original situation may lead to
	/// opposite decision at next frame.
	void Integrate(KyFloat32 simulationTimeInSeconds, const Vec2f& avoidanceVelocity, const Vec2f& desiredTargetPosition, KyFloat32 desiredSpeed);


	// ---------------------------------- Public Data Members ----------------------------------

	/// The VelocityObstacleCollider corresponding to the Bot for which the
	/// VelocityObstacleWorkspace is used to compute an avoidance velocity.
	VelocityObstacleCollider m_rootCollider;

	/// Potential collider filtering parameter. A potential collider will be
	/// taken into account only if he may be close enough to collide in
	/// #m_minimalTimeToCollision seconds.
	KyFloat32 m_minimalTimeToCollision;

	/// The array of retained VelocityObstacle against which the the candiate
	/// velocities will be validated.
	VelocityObstacleArray m_velocityObstacles;

	/// The array of Colliders which VelocityObstacle have been kept in #m_velocityObstacles.
	ColliderArray m_colliders;

	/// The array of discarded Colliders. They are namely used in Integrate()
	/// function to complement #m_velocityObstacles with the ones that should
	/// enter the focus area in next frame.
	ColliderArray m_ignoredColliders;


public: //internal

	// used to activate the VelocityObstacleWorkspaceBlob to be sent when visual
	// debugging a Bot in VisualDebugLOD_Maximal.
	bool m_doSendVisualDebugBlob;
};

} // namespace Kaim

#endif // Navigation_VelocityObstacleWorkspace_H
