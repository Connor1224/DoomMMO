/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



namespace Kaim
{

template<class NavTagTraversePredicate>
inline bool OrientVelocityToClosestSafeDirection<NavTagTraversePredicate>::TestDirection(
	const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition, const NavTrianglePtr& botTrianglePtr,
	const Vec2f& testedDirection, KyFloat32 originalSpeed)
{
	if( workspace.IsVelocitySafe(testedDirection * originalSpeed) )
	{
		if( database )
		{
			RayCastQuery<NavTagTraversePredicate> rayCastQuery(m_predicate);

			rayCastQuery.Initialize(database, botPosition, testedDirection, workspace.m_rootCollider.m_radius*1.05f);
			rayCastQuery.SetStartTrianglePtr(botTrianglePtr);
			rayCastQuery.SetPositionSpatializationRange(PositionSpatializationRange(KyFloat32MAXVAL, KyFloat32MAXVAL));
			rayCastQuery.PerformQuery();

			RayCastQueryResult queryResult = rayCastQuery.GetResult();

			if(queryResult == RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED) 
			{
				return true;
			} 
		} else {
			return true;
		}
	}
	return false;
}

template<class NavTagTraversePredicate>
inline bool OrientVelocityToClosestSafeDirection<NavTagTraversePredicate>::FindSafeVelocityInRange(
	const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition, const NavTrianglePtr& botTrianglePtr, 
	const Vec2f& from, const Vec2f& to, KyFloat32 originalSpeed, Vec2f& result)
{
	KyFloat32 fAngleIncrement = m_angleSpan / m_sampleCount;

	Vec2f planeNormal      = Vec2f(-to.y, to.x); 
	Vec2f desiredDirection = from;  desiredDirection.Normalize();

	KyFloat32 rotationSign = DotProduct(planeNormal, desiredDirection);

	KyFloat32 rotationCos = cosf( rotationSign < 0.0f ? fAngleIncrement : (KyFloat32)(KY_MATH_PI * 2.0f) - fAngleIncrement);
	KyFloat32 rotationSin = sinf( rotationSign < 0.0f ? fAngleIncrement : (KyFloat32)(KY_MATH_PI * 2.0f) - fAngleIncrement);

	Vec2f testedDirection = desiredDirection; 

	while((rotationSign < 0.0f && DotProduct(planeNormal, testedDirection) < 0.0f ) || (rotationSign > 0.0f && DotProduct(planeNormal, testedDirection) > 0.0f ))
	{
		testedDirection = Vec2f( (testedDirection[0] * rotationCos) - (testedDirection[1] * rotationSin), (testedDirection[0] * rotationSin) + (testedDirection[1] * rotationCos)); 
		testedDirection.Normalize();
		if( TestDirection(workspace, database, botPosition, botTrianglePtr, testedDirection, originalSpeed) )
		{
			result = testedDirection * originalSpeed;
			return true;
		}
	}
	return false;
}


template<class NavTagTraversePredicate>
inline bool OrientVelocityToClosestSafeDirection<NavTagTraversePredicate>::FindSafeVelocity(
	bool enableBackMove, const VelocityObstacleWorkspace& workspace, Database* database, const Vec3f& botPosition,
	const NavTrianglePtr& botTrianglePtr, const Vec2f& previousVelocity, const Vec2f& desiredVelocity, Vec2f& result)
{
	RayCastQuery<NavTagTraversePredicate> rayCastQuery(m_predicate);

	Vec2f normalizedVelocity = m_originalVelocity;
	KyFloat32 originalSpeed = normalizedVelocity.Normalize();

	Vec2f previousDirection      = previousVelocity;
	const KyFloat32 previousSpeed = previousDirection.Normalize();
	Vec2f desiredDirection       = desiredVelocity;
	desiredDirection.Normalize();
	Vec2f desiredDirectionNormal = Vec2f(-desiredDirection.y, desiredDirection.x);

	if( DotProduct(desiredDirectionNormal, previousDirection) < 0.0f ) 
	{
		desiredDirectionNormal *= -1.0f;
	}

	bool safeDirection = true;
	if( DotProduct(desiredDirection, previousDirection) < 0.0f ) 
	{
		// We want to favor moving forward toward the objective, therefore we inverse the previousVelocity
		previousDirection *= -1.0f;
		safeDirection = false;
	}

	if( previousSpeed != 0.0f )
	{
		// Hunt for a valid velocity between the desired velocity and the previous velocity range
		if( FindSafeVelocityInRange(workspace, database, botPosition, botTrianglePtr, desiredDirection, previousDirection, originalSpeed, result ) )
		{
			Vec2f foundDirection = result;
			foundDirection.Normalize();
			if( enableBackMove || DotProduct(desiredDirectionNormal, foundDirection) > 0.0f ) 
			{
				return true;
			}
		}

		// Let's try to stay on the previously chosen side to avoid changing direction turn
		desiredDirection  = previousDirection;
		previousDirection = desiredDirectionNormal; 

		if( FindSafeVelocityInRange(workspace, database, botPosition, botTrianglePtr, desiredDirection, previousDirection, originalSpeed, result ) )
		{
			Vec2f foundDirection = result;
			foundDirection.Normalize();
			if( enableBackMove || DotProduct(desiredDirectionNormal, foundDirection) > 0.0f ) 
			{
				return true;
			}
		}
		
		if( safeDirection )
		{
			// Let's try to stay stable then
			Vec2f testedDirection = previousDirection; 
			if( TestDirection(workspace, database, botPosition, botTrianglePtr, testedDirection, originalSpeed) )
			{
				result = testedDirection * originalSpeed;
				return true;
			} 
		}
	}

	// Ok, no solutions there, so let's hunt for the shortest rotation angle possible
	KyFloat32 fAngleLimit = m_angleSpan * 0.5f; 
	if(( !enableBackMove ) && (fAngleLimit > (((KyFloat32)(KY_MATH_PI) - 0.1f) * 0.5f)))
	{
		fAngleLimit = (((KyFloat32)(KY_MATH_PI) - 0.1f) * 0.5f);
	}
	KyFloat32 fAngleIncrement = m_angleSpan / m_sampleCount;

	Vec2f right  = normalizedVelocity;
	Vec2f left   = normalizedVelocity;
	KyFloat32 fAngle = fAngleIncrement;

	KyFloat32 fRightCos = cosf(fAngle);
	KyFloat32 fRightSin = sinf(fAngle);

	KyFloat32 fLeftCos = cosf((KyFloat32)(KY_MATH_PI * 2.0f) - fAngle);
	KyFloat32 fLeftSin = sinf((KyFloat32)(KY_MATH_PI * 2.0f) - fAngle);

	while(fAngle < fAngleLimit)
	{
		right = Vec2f( (right[0] * fRightCos) - (right[1] * fRightSin), (right[0] * fRightSin) + (right[1] * fRightCos)); 
		left  = Vec2f( (left[0] * fLeftCos) - (left[1] * fLeftSin), (left[0] * fLeftSin) + (left[1] * fLeftCos)); 

		right.Normalize();
		left.Normalize();

		if( TestDirection(workspace, database, botPosition, botTrianglePtr, right, originalSpeed) )
		{
			result = right * originalSpeed;
			return true;
		} 

		if( TestDirection(workspace, database, botPosition, botTrianglePtr, left, originalSpeed) )
		{
			result = left * originalSpeed;
			return true;
		}

		fAngle += fAngleIncrement;
	}

	return false;
}



}
