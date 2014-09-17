/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY

namespace Kaim
{
namespace
{
	class ScopeRetrievePreviousPosition
	{
	public:
		ScopeRetrievePreviousPosition(Bot* bot) : m_bot(bot) {}
		~ScopeRetrievePreviousPosition()
		{
			// Remember current state for next frame
			m_bot->m_previousPosition = m_bot->m_spatializedCylinder.GetPosition();
		}
	public:
		Bot* m_bot;
	};
}


template <class NavTagTraversePredicate>
inline void PathFollower<NavTagTraversePredicate>::Update(Bot* bot, KyFloat32 simulationTimeInSeconds)
{
	// Automatically set bot->m_previousPosition to bot current position when returning from this function
	ScopeRetrievePreviousPosition scopeRetrievePreviousPosition(bot);

	// Let's make sure that we have a path we can follow.
	// When a new path is available, replace the current path with the new one
	// and set the target on path to new path first node.
	UpdateLivePath(bot);

	if (bot->GetFollowedPath() == KY_NULL)
		return;

	if (bot->DoComputeTargetOnPath())
		ComputeTargetOnPath(bot);

	if (bot->DoComputeTrajectory())
		bot->ComputeTrajectory(simulationTimeInSeconds);
}

template <class NavTagTraversePredicate>
inline void PathFollower<NavTagTraversePredicate>::UpdateLivePath(Bot* bot)
{
	bool pathHasChanged = false;
	if (bot->m_livePath.NeedToReplaceCurrentPathWithNewPath())
	{
		pathHasChanged = true;

		if (bot->GetPathEventList().GetPathEventCount() != 0)
			bot->m_pathEventListObserver->OnPathEventListDestroy(bot, bot->GetPathEventList(), IPathEventListObserver::ReplacePathWithNewPath);

		bot->m_livePath.ReplaceCurrentPathWithNewPath();
	}

	Path* path = bot->m_livePath.GetFollowedPath();

	if (path == KY_NULL)
	{
		bot->m_targetOnLivePath.Clear();
		bot->m_targetOnPathStatus = TargetOnPathNotInitialized;
		return;
	}

	if (pathHasChanged)
	{
		bot->m_targetOnLivePath.InitOnPathFirstNode();
		bot->m_targetOnPathStatus = TargetOnPathUnknownReachability;
	}

	// Perform an interval validity step if needed
	switch(bot->m_livePath.GetPathValidityStatus())
	{
	case ValdityIntervalMustBeRecomputed:
		{
			if (bot->GetPathEventList().GetPathEventCount() != 0)
				bot->m_pathEventListObserver->OnPathEventListDestroy(bot, bot->GetPathEventList(), IPathEventListObserver::RecomputePathEventListForSamePath);

			//What to do when the path may be dirty (after event broadCast?)
			// restart scouting from targetOnPath. 1 step backward and 1 step forward
			bot->m_livePath.ReStartIntervalComputation<NavTagTraversePredicate>(bot->m_targetOnLivePath.GetPositionOnPath(),
				bot->GetPathFollowerConfig().m_pathValidationDistanceBackward, bot->GetPathFollowerConfig().m_pathValidationDistanceForward, &m_predicate);

			if (bot->GetPathEventList().GetPathEventCount() != 0)
			{
				bot->m_targetOnLivePath.FindLocationOnEventListFromScratch();
				bot->m_pathEventListObserver->OnPathEventListBuldingStageDone(bot, bot->GetPathEventList(), 0);
			}

			break;
		}
	case ValidityIsBeingChecked:
	case Dirty_InvalidBeforeTarget_ProcessingAfter :
		{
			const KyUInt32 previousCount = bot->GetPathEventList().GetPathEventCount();

			//What to do when the path were tagged as "may be dirty" but its full validity check is not finished
			// go on scouting : 1 step forward
			bot->m_livePath.ValidateForward<NavTagTraversePredicate>(bot->GetPathFollowerConfig().m_pathValidationDistanceForward, &m_predicate);

			KY_LOG_ERROR_IF(previousCount > bot->GetPathEventList().GetPathEventCount() ,("We should have created node, and not delete them !"));

			if (bot->GetPathEventList().GetPathEventCount() != previousCount)
				bot->m_pathEventListObserver->OnPathEventListBuldingStageDone(bot, bot->GetPathEventList(), previousCount);

			break;
		}
	default :
		break;
	}

	if (bot->m_livePath.GetPathValidityStatus() == Dirty_TargetInvalid ||
		bot->m_targetOnLivePath.GetOnEventListStatus() == OnEventListStatus_Undefined ||
		bot->m_targetOnLivePath.GetOnEventListIndex() == KyUInt32MAXVAL)
	{
		bot->m_targetOnPathStatus = TargetOnPathInInvalidNavData;
		return;
	}

	KY_LOG_ERROR_IF(bot->GetPathEventList().GetPathEventCount() == 0, ("Error, if the pathEventlist is empty, no following can be process"));
	KY_ASSERT(bot->m_targetOnLivePath.GetPath() == path);
}

template <class NavTagTraversePredicate>
inline KyResult PathFollower<NavTagTraversePredicate>::ComputeTargetOnPath(Bot* bot)
{
	KY_SCOPED_PERF_MARKER("PathFollower::ComputeTargetOnPath");

	if (bot->GetTargetOnLivePathStatus() == TargetOnPathInInvalidNavData)
		return KY_ERROR;

	KY_ASSERT(bot->m_targetOnLivePath.GetOnEventListStatus() != OnEventListStatus_Undefined && bot->m_targetOnLivePath.GetOnEventListIndex() != KyUInt32MAXVAL);

	ComputeTargetOnPathSearchControl forwardSearchControl = ContinueSearch;
	if (bot->m_targetOnLivePath.GetOnEventListStatus() == OnEventListStatus_OnEvent)
	{
		const PathEvent& pathEvent = bot->GetPathEventList().GetPathEvent(bot->m_targetOnLivePath.GetOnEventListIndex());
		if (pathEvent.IsOnUpperBound())
			forwardSearchControl = StopSearch;
		else if (pathEvent.IsACheckPoint())
		{
			// Target on path has been stopped on a check point. We do not move it forward until it is validated.
			if (m_targetOnPathCheckPointValidator->Validate(bot, bot->m_targetOnLivePath.GetPositionOnPath()) == false)
				forwardSearchControl = StopSearch;
		}
	}

	return m_targetOnPathComputer.ComputeTargetOnPath(bot, forwardSearchControl);
}


}
