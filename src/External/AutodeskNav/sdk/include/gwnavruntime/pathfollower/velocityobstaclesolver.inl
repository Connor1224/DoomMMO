/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

namespace Kaim
{
template<class NavTagTraversePredicate>
IAvoidanceComputer::AvoidanceResult VelocityObstacleSolver<NavTagTraversePredicate>::Solve( VelocityObstacleSolverConfig& config, const Vec3f& previousVelocity, const Vec3f& desiredVelocity, Vec3f& outputVelocity )
{
	const Vec3f& botPosition = config.m_colliderSpatializedPoint->GetPosition();
	const NavTrianglePtr botTrianglePtr = config.m_colliderSpatializedPoint->GetNavTrianglePtr(config.m_database);
	const KyFloat32 desiredValidityDistance = config.m_deltaTime * desiredVelocity.GetLength();

	RayCastQuery<NavTagTraversePredicate> rayCastQuery(m_predicate);

	outputVelocity = desiredVelocity;

	// 1 - Try the velocity going to the target at full speed
	if( config.m_workspace->IsVelocitySafe(outputVelocity.Get2d()) )
	{
		return IAvoidanceComputer::NoAvoidance;
	}

	// 2 - Try to stay on the same direction at slow speed
	Vec2f result = outputVelocity.Get2d();
	m_scaleVelocityToSafeSpeed.m_maximalDesiredVelocity = outputVelocity.Get2d();
	if( config.m_avoidanceComputerConfig->m_enableSlowingDown && m_scaleVelocityToSafeSpeed.FindSafeVelocity(*config.m_workspace, result) )
	{
		outputVelocity *= (result.GetLength() / outputVelocity.GetLength2d());
		return IAvoidanceComputer::SlowDown;
	}

	// 3 - Try a different direction
	result                                                    = desiredVelocity.Get2d();
	m_orientVelocityToClosestSafeDirection.m_originalVelocity = desiredVelocity.Get2d();

	Vec2f desiredVelocity2d  = desiredVelocity.Get2d();
	Vec2f previousVelocity2d = previousVelocity.Get2d();
	if( m_orientVelocityToClosestSafeDirection.FindSafeVelocity(config.m_avoidanceComputerConfig->m_enableBackMove, *config.m_workspace, config.m_database, botPosition, botTrianglePtr, previousVelocity2d, desiredVelocity2d, result) )
	{
		Vec2f direction = result;
		const KyFloat32 resultLength2d = direction.Normalize();

		rayCastQuery.Initialize(config.m_database, botPosition, direction, config.m_workspace->m_rootCollider.m_radius * 1.05f);
		rayCastQuery.SetStartTrianglePtr(botTrianglePtr);
		rayCastQuery.SetPositionSpatializationRange(PositionSpatializationRange(KyFloat32MAXVAL, KyFloat32MAXVAL));
		rayCastQuery.PerformQuery();

		RayCastQueryResult queryResult = rayCastQuery.GetResult();

		if (queryResult == RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED)
		{
			Vec3f avoidancePosition = rayCastQuery.GetArrivalPos();
			outputVelocity.Set(result.x, result.y, (resultLength2d / desiredValidityDistance) * (avoidancePosition.z - botPosition.z));
			return IAvoidanceComputer::Turn;
		}
	}

	// 4 - Oh well, give up then...
	outputVelocity = config.m_avoidanceComputerConfig->m_enableStop ? Vec3f::Zero() : desiredVelocity;
	if( config.m_avoidanceComputerConfig->m_enableStop && config.m_workspace->IsVelocitySafe(outputVelocity.Get2d()) )
	{
		return IAvoidanceComputer::Stop;
	}
	return IAvoidanceComputer::SolutionNotFound;
}

}
