/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_TargetOnPathComputer_H
#define Navigation_TargetOnPathComputer_H


#include "gwnavruntime/world/world.h"
#include "gwnavruntime/path/patheventlist.h"
#include "gwnavruntime/database/database.h"
#include "gwnavruntime/queries/raycangoquery.h"
#include "gwnavruntime/queries/findfirstvisiblepositiononpathquery.h"
#include "gwnavruntime/visualdebug/visualdebugserver.h"
#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/containers/stringstream.h"
#include "gwnavruntime/spatialization/spatializedcylinder.h"
#include "gwnavruntime/world/bot.h"

namespace Kaim
{

enum ComputeTargetOnPathResult
{
	KeepCurrentTarget = 0,
	BetterTargetFound,
	NoBetterTargetFound
};

enum ShortcutOnNextSampleValidationControl
{
	CallValidateCandidate,
	ForceCandiateValidStatus
};

enum ComputeTargetOnPathSearchControl
{
	ContinueSearch = 0,
	StopSearch
};


/// This class updates Bot's target on path so that:
///  - it is always directly reachable from Bot current position in straight line
///  - it is less than a maximal distance from the Bot.
/// If the current target on path is not directly reachable, it searches a new
/// target on path backwards. This case happens when the Trajectory or the game engine
/// moved the Bot in another direction than towards its target on path.
/// All the parameters to control this computer are in TargetOnPathConfig.
/// If it can't manage to find a valid target on path, the Bot TargetOnPathStatus is
// set to TargetOnPathNotReachable. No further action is done, you have to check the
/// status and take your own decision (compute a path to the same destination, pick
/// a new destination...).
template <class NavTagTraversePredicate>
class TargetOnPathComputer
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	TargetOnPathComputer() : m_predicate() {}
	TargetOnPathComputer(const NavTagTraversePredicate& predicate) : m_predicate(predicate) {}

	KyResult ComputeTargetOnPath(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl);

protected:
	enum ValidationResult
	{
		CandidateIsValid = 0,
		InvalidCandidate_CanGoFailure,
		InvalidCandidate_TooFarFromBot
	};

	KyResult ComputeWhenNotOnMesh(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl);
	KyResult ComputeWhenOnMesh(Bot* bot, ComputeTargetOnPathSearchControl forwardSearchControl);

	ComputeTargetOnPathResult Shortcut(Bot* bot);
	KyResult MakeSureTargetOnPathIsVisible(Bot* bot);
	KyResult CheckTargetOnPathIsVisible(Bot* bot);
	KyResult FindVisibleTargetOnPathBackward(Bot* bot);
	ComputeTargetOnPathSearchControl ShortcutOnNextSample(Bot* bot, PositionOnLivePath& candidate, PositionOnLivePath& bestCandidate, ShortcutOnNextSampleValidationControl validationControl);
	ValidationResult ValidateCandidate(Bot* bot, const Vec3f& candidatePosition);

protected:
	NavTagTraversePredicate m_predicate;
};


} // namespace Kaim


#include "gwnavruntime/pathfollower/targetonpathcomputer.inl"

#endif // Navigation_TargetOnPathComputer_H
