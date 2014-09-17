/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_PathFollower_H
#define Navigation_PathFollower_H

#include "gwnavruntime/pathfollower/basepathfollower.h"
#include "gwnavruntime/pathfollower/targetonpathcomputer.h"
#include "gwnavruntime/world/bot.h"
#include "gwnavruntime/path/ipatheventlistobserver.h"
#include "gwnavruntime/basesystem/iperfmarkerinterface.h"

namespace Kaim
{


/// Default implementation of BasePathFollower interface that use a
/// NavTagTraversePredicate to control Bot navigation abilities.
template <class NavTagTraversePredicate>
class PathFollower: public BasePathFollower
{
	KY_CLASS_WITHOUT_COPY(PathFollower)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	PathFollower() : BasePathFollower(), m_predicate(), m_targetOnPathComputer() {}
	PathFollower(const NavTagTraversePredicate& predicate)
		: BasePathFollower()
		, m_predicate(predicate)
		, m_targetOnPathComputer(predicate)
	{}

	virtual ~PathFollower() {}

	virtual void Update(Bot* bot, KyFloat32 simulationTimeInSeconds);

private:
	void UpdateLivePath(Bot* bot);
	KyResult ComputeTargetOnPath(Bot* bot);


	// ---------------------------------- Public Data Members ----------------------------------

	NavTagTraversePredicate m_predicate;
	TargetOnPathComputer<NavTagTraversePredicate> m_targetOnPathComputer;
};

} // namespace Kaim

#include "gwnavruntime/pathfollower/pathfollower.inl"

#endif // Navigation_PathFollower_H
