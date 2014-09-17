/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: LAPA


namespace Kaim
{

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::ComputeTargetOnPath(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl)
{
	const PositionOnLivePath& targetOnPath = bot->GetTargetOnLivePath();

	const KyUInt32 currentEdgeIdx = targetOnPath.IsAtLowerBoundOfLivePath() ? targetOnPath.GetEdgeIdxToMoveOnForward() : targetOnPath.GetEdgeIdxToMoveOnBackward();
	const PathEdgeType currentEdgeType = targetOnPath.GetPath()->GetPathEdgeType(currentEdgeIdx);
	if (currentEdgeType == PathEdgeType_OnNavMesh)
	{
		// Ensure current target on path is still visible
		KY_FORWARD_ERROR_NO_LOG(MakeSureTargetOnPathIsVisible(bot));
	}

	if (targetOnPath.IsAtUpperBoundOfLivePath())
		return KY_SUCCESS;

	const PathEdgeType nextEdgeType = targetOnPath.GetPath()->GetPathEdgeType(targetOnPath.GetEdgeIdxToMoveOnForward());
	if (nextEdgeType == PathEdgeType_OnNavMesh)
		return ComputeWhenOnMesh(bot, forwardSearchControl);
	else
		return ComputeWhenNotOnMesh(bot, forwardSearchControl);
}

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::ComputeWhenNotOnMesh(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl)
{
	// In not in EdgeOnMesh case, we simply move the target on path to next PathEvent
	if (forwardSearchControl == ContinueSearch)
		bot->m_targetOnLivePath.MoveForwardToNextPathEvent();

	return KY_SUCCESS;
}

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::ComputeWhenOnMesh(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl)
{
	if (forwardSearchControl == ContinueSearch)
		Shortcut(bot);

	return KY_SUCCESS;
}

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::MakeSureTargetOnPathIsVisible(Bot* bot)
{
	if (CheckTargetOnPathIsVisible(bot) == KY_SUCCESS)
		return KY_SUCCESS;

	// Target on path is really not reachable ! => if possible, try to
	// backtrack it along the Path.
	bot->m_targetOnPathStatus = TargetOnPathNotReachable;

	return FindVisibleTargetOnPathBackward(bot);
}

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::CheckTargetOnPathIsVisible(Bot* bot)
{
	PositionOnLivePath& targetOnPath = bot->m_targetOnLivePath;
	const NavTrianglePtr& botNavTrianglePtr = bot->GetNavTrianglePtr();
	Database* database = bot->GetDatabase();
	const Vec3f& botPos = bot->GetPosition();
	const Vec3f& targetPos = targetOnPath.GetPositionOnPath().GetPosition();

	RayCanGoQuery<NavTagTraversePredicate> canGoQuery(m_predicate);

	const DisplayListActivation displayListActivation = ((bot->GetCurrentVisualDebugLOD() == VisualDebugLOD_Maximal) ? DisplayList_Enable : DisplayList_Disable);
	ScopedDisplayList displayList(database->GetWorld(), "MakeSureTargetOnPathIsVisible", "TargetOnPathComputer", displayListActivation);
	displayList.HideByDefaultInNavigationLab();

	canGoQuery.Initialize(database, botPos, targetPos);
	canGoQuery.SetStartTrianglePtr(botNavTrianglePtr);
	canGoQuery.PerformQuery();

	VisualColor color = canGoQuery.GetResult() == RAYCANGO_DONE_SUCCESS ? VisualColor::Lime : VisualColor::Red;
	displayList.PushLine(canGoQuery.GetStartPos(), canGoQuery.GetDestPos(), color);
	displayList.PushLine(canGoQuery.GetDestPos(), canGoQuery.GetDestPos() + Vec3f::UnitZ(), color);

	if(canGoQuery.GetResult() == Kaim::RAYCANGO_DONE_SUCCESS)
	{
		bot->m_targetOnPathStatus = TargetOnPathReachable;
		return KY_SUCCESS;
	}

	// The target on path can be not reachable due to recent forced forward movement
	// because previous target has been reached. Let validate with slight side steps
	// on left and right to be sure it is actually not reachable.

	Vec2f rightOffset;
	(targetPos - botPos).Get2d().PerpCW().GetNormalized(rightOffset);
	rightOffset *= bot->GetPathFollowerConfig().m_shortcutForceTOPForwardDistance;

	// - slightly on the right
	canGoQuery.Initialize(database, botPos, targetPos + rightOffset);
	canGoQuery.SetStartTrianglePtr(botNavTrianglePtr);
	canGoQuery.PerformQuery();

	color = canGoQuery.GetResult() == RAYCANGO_DONE_SUCCESS ? VisualColor::Lime : VisualColor::Red;
	displayList.PushLine(canGoQuery.GetStartPos(), canGoQuery.GetDestPos(), color);
	displayList.PushLine(canGoQuery.GetDestPos(), canGoQuery.GetDestPos() + Vec3f::UnitZ(), color);

	if(canGoQuery.GetResult() == Kaim::RAYCANGO_DONE_SUCCESS)
	{
		bot->m_targetOnPathStatus = TargetOnPathReachable;
		return KY_SUCCESS;
	}

	// - slightly on the left
	canGoQuery.Initialize(database, botPos, targetPos - rightOffset);
	canGoQuery.SetStartTrianglePtr(botNavTrianglePtr);
	canGoQuery.PerformQuery();

	color = canGoQuery.GetResult() == RAYCANGO_DONE_SUCCESS ? VisualColor::Lime : VisualColor::Red;
	displayList.PushLine(canGoQuery.GetStartPos(), canGoQuery.GetDestPos(), color);
	displayList.PushLine(canGoQuery.GetDestPos(), canGoQuery.GetDestPos() + Vec3f::UnitZ(), color);

	if(canGoQuery.GetResult() == Kaim::RAYCANGO_DONE_SUCCESS)
	{
		bot->m_targetOnPathStatus = TargetOnPathReachable;
		return KY_SUCCESS;
	}

	return KY_ERROR;
}

template <class NavTagTraversePredicate>
inline KyResult TargetOnPathComputer<NavTagTraversePredicate>::FindVisibleTargetOnPathBackward(Bot* bot)
{
	PositionOnLivePath& targetOnPath = bot->m_targetOnLivePath;

	if (targetOnPath.IsAtLowerBoundOfLivePath())
		return KY_ERROR;

	// Cannot can go to target on path => Find backward first visible target on path
	const KyFloat32 backwardMaxDistance = bot->GetTargetOnPathConfig().m_backwardMaxDistance;
	const KyFloat32 backwardSamplingDistance = bot->GetTargetOnPathConfig().m_backwardSamplingDistance;

	PositionOnLivePath backwardTOPLimit = targetOnPath;
	backwardTOPLimit.MoveBackward_StopAtEvent(backwardMaxDistance);

	const DisplayListActivation displayListActivation = ((bot->GetCurrentVisualDebugLOD() == VisualDebugLOD_Maximal) ? DisplayList_Enable : DisplayList_Disable);
	ScopedDisplayList displayList(bot->GetWorld(), "BackwardSearch", "TargetOnPathComputer", displayListActivation);
	displayList.HideByDefaultInNavigationLab();

	FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate> findFirstVisiblePositionOnPathQuery(m_predicate);
	findFirstVisiblePositionOnPathQuery.Initialize(bot->GetDatabase(), targetOnPath.GetPositionOnPath(), backwardTOPLimit.GetPositionOnPath(), bot->GetPosition());
	findFirstVisiblePositionOnPathQuery.SetVisibilityStartTrianglePtr(bot->GetNavTrianglePtr());
	findFirstVisiblePositionOnPathQuery.SetSamplingDistance(backwardSamplingDistance);
	findFirstVisiblePositionOnPathQuery.m_displayList = &displayList;

	findFirstVisiblePositionOnPathQuery.PerformQuery();

	if (findFirstVisiblePositionOnPathQuery.GetResult() == FINDFIRSTVISIBLEPOSITIONONPATH_DONE_SUCCESS)
	{
		targetOnPath.SetNewPositionOnPathBackward(findFirstVisiblePositionOnPathQuery.GetFirstVisiblePositionOnPath());
		bot->m_targetOnPathStatus = TargetOnPathReachable;
		return KY_SUCCESS;
	}

	return KY_ERROR;
}

template <class NavTagTraversePredicate>
inline ComputeTargetOnPathResult TargetOnPathComputer<NavTagTraversePredicate>::Shortcut(Bot* bot)
{
	const PathFollowerConfig& pathFollowerConfig = bot->GetPathFollowerConfig();
	const TargetOnPathConfig& targetOnPathConfig = pathFollowerConfig.m_targetOnPathConfig;

	const Vec3f& botPosition = bot->GetPosition();
	PositionOnLivePath& targetOnPath = bot->m_targetOnLivePath;
	const KyFloat32 sqDistToCurrentTargetOnPath = SquareDistance2d(botPosition, targetOnPath.GetPosition());

	if (bot->m_forceTargetOnPathComputationOnNextUpdate == false)
	{
		// When computation is not forced, we do it only when the TOP is
		// - either close to Bot (less than m_shortcutForceAttemptDistance)
		// - or the Bot sufficiently moved toward the TOP
		const KyFloat32 shortcutForceAttemptSquareDistance = targetOnPathConfig.m_shortcutForceAttemptDistance * targetOnPathConfig.m_shortcutForceAttemptDistance;
		if (sqDistToCurrentTargetOnPath > Max(shortcutForceAttemptSquareDistance, bot->m_nextShortcutSqDistTrigger))
			return KeepCurrentTarget;
	}

	bot->m_forceTargetOnPathComputationOnNextUpdate = false;

	if (sqDistToCurrentTargetOnPath < pathFollowerConfig.m_shortcutForceTOPForwardDistance * pathFollowerConfig.m_shortcutForceTOPForwardDistance)
		ShortcutOnNextSample(bot, targetOnPath, targetOnPath, ForceCandiateValidStatus);

	PositionOnLivePath candidate = targetOnPath;
	PositionOnLivePath bestCandidate; // Don't initialize it!
	while (candidate.IsAtUpperBoundOfLivePath() == false) 
	{
		PositionOnLivePath previousCandidate     = candidate;
		PositionOnLivePath previousBestCandidate = bestCandidate;
		if (ShortcutOnNextSample(bot, candidate, bestCandidate, CallValidateCandidate) != ContinueSearch)
		{
			if(!bot->IsShortcutEnabled())
			{
				if( candidate.GetPathEdgeIndex() != previousCandidate.GetPathEdgeIndex() )
				{
					candidate = previousCandidate;
					bestCandidate = previousBestCandidate;
				}
			}
			break;
		}
		if(!bot->IsShortcutEnabled())
		{
			if( candidate.GetPathEdgeIndex() != previousCandidate.GetPathEdgeIndex() ) 
			{
				candidate = previousCandidate;
				bestCandidate = previousBestCandidate;
				break;
			}
		}
	}


	if (bestCandidate.GetPositionOnPathStatus() != PositionOnPathStatus_Undefined)
	{
		targetOnPath = bestCandidate;
		bot->m_nextShortcutSqDistTrigger = targetOnPathConfig.m_shortcutAttemptDistanceRatio * SquareDistance(botPosition, targetOnPath.GetPosition());
		return BetterTargetFound;
	}

	bot->m_nextShortcutSqDistTrigger = targetOnPathConfig.m_shortcutAttemptDistanceRatio * sqDistToCurrentTargetOnPath;
	return NoBetterTargetFound;
}

template <class NavTagTraversePredicate>
inline ComputeTargetOnPathSearchControl
	TargetOnPathComputer<NavTagTraversePredicate>::ShortcutOnNextSample(
	Bot* bot, PositionOnLivePath& candidate, PositionOnLivePath& bestCandidate, ShortcutOnNextSampleValidationControl validationControl)
{
	const KyFloat32 samplingDistance = bot->GetTargetOnPathConfig().m_samplingDistance;
	candidate.MoveForward_StopAtEventOrAtPathNode(samplingDistance);

	if (validationControl == CallValidateCandidate)
	{
		const ValidationResult validationResult = ValidateCandidate(bot, candidate.GetPosition());
		if (validationResult != CandidateIsValid)
			return StopSearch;
	}

	bestCandidate = candidate;

	if (candidate.GetOnEventListStatus() == OnEventListStatus_OnEvent)
	{
		const PathEvent& pathEvent = bot->GetPathEventList().GetPathEvent(candidate.GetOnEventListIndex());
		if (pathEvent.IsACheckPoint() || pathEvent.IsOnUpperBound())
			return StopSearch;
	}

	return ContinueSearch;
}

template<class NavTagTraversePredicate>
inline typename TargetOnPathComputer<NavTagTraversePredicate>::ValidationResult
	TargetOnPathComputer<NavTagTraversePredicate>::ValidateCandidate(
	Bot* bot, const Vec3f& candidatePosition)
{
	Database* database = bot->GetDatabase();
	const Vec3f& botPosition = bot->GetPosition();
	const NavTrianglePtr& botNavTrianglePtr = bot->GetNavTrianglePtr();

	const KyFloat32 maxDistanceFromBot = bot->GetTargetOnPathConfig().m_maxDistanceFromBot;
	if (SquareDistance(botPosition, candidatePosition) > maxDistanceFromBot * maxDistanceFromBot)
		return InvalidCandidate_TooFarFromBot;

	RayCanGoQuery<NavTagTraversePredicate> canGoQuery(m_predicate);
	canGoQuery.Initialize(database, botPosition, candidatePosition);
	canGoQuery.SetStartTrianglePtr(botNavTrianglePtr);
	canGoQuery.PerformQuery();

	// Visual debug the candidate validation
	const DisplayListActivation displayListActivation = ((bot->GetCurrentVisualDebugLOD() == VisualDebugLOD_Maximal) ? DisplayList_Enable : DisplayList_Disable);
	ScopedDisplayList displayList(bot->GetWorld(), "CandidateValidatation", "TargetOnPathComputer", displayListActivation);
	displayList.HideByDefaultInNavigationLab();
	const VisualColor& color = canGoQuery.GetResult() == RAYCANGO_DONE_SUCCESS ? VisualColor::Lime : VisualColor::Red;
	displayList.PushLine(canGoQuery.GetStartPos(), canGoQuery.GetDestPos(), color);
	displayList.PushLine(canGoQuery.GetDestPos(), canGoQuery.GetDestPos() + Vec3f::UnitZ(), color);

	return ((canGoQuery.GetResult() == Kaim::RAYCANGO_DONE_SUCCESS) ? CandidateIsValid : InvalidCandidate_CanGoFailure);
}

} // namespace Kaim
