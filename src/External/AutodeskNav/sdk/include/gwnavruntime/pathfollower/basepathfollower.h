/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_BasePathFollower_H
#define Navigation_BasePathFollower_H

#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/pathfollower/ipositiononpathvalidator.h"

namespace Kaim
{

class Bot;


/// The base class for all Path follower classes.
/// We provide a PathFollower class that integrate a NavTagTraversePredicate.
/// You can implement your own class for specific path following.
class BasePathFollower: public RefCountBase<BasePathFollower, MemStat_PathFollowing>
{
	KY_CLASS_WITHOUT_COPY(BasePathFollower)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	BasePathFollower() {}
	virtual ~BasePathFollower() {}

	/// Sets the IPositionOnPathValidator to be used to validate target on path check points.
	void SetTargetOnPathCheckPointValidator(const Ptr<IPositionOnPathValidator>& targetOnPathCheckPointValidator);


	// ---------------------------------- Pure Virtual Functions ----------------------------------

	virtual void Update(Bot* bot, KyFloat32 simulationTimeInSeconds) = 0;


protected:
	Ptr<IPositionOnPathValidator> m_targetOnPathCheckPointValidator;
};


KY_INLINE void BasePathFollower::SetTargetOnPathCheckPointValidator(const Ptr<IPositionOnPathValidator>& targetOnPathCheckPointValidator) { m_targetOnPathCheckPointValidator = targetOnPathCheckPointValidator; }

} // namespace Kaim

#endif // Navigation_IPathFollower_H
