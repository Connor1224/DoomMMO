/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_NavTagMovePositionOnPathPredicate_H
#define Navigation_NavTagMovePositionOnPathPredicate_H

#include "gwnavruntime/pathfollower/imovepositiononpathpredicate.h"

namespace Kaim
{

class NavTag;


/// Default implementation of IMovePositionOnPathPredicate interface based on NavTag.
class NavTagMovePositionOnPathPredicate : public IMovePositionOnPathPredicate
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	NavTagMovePositionOnPathPredicate(const NavTag* navTag) : m_navTag(navTag) {}
	virtual ~NavTagMovePositionOnPathPredicate() {}

	/// \returns true if positionOnPath is on or at start of a PathEventInterval
	/// with the same NavTag value than #m_navTag.
	virtual bool CanMoveForward(Bot* bot, PositionOnLivePath& positionOnLivePath);
	virtual bool CanMoveBackward(Bot* bot, PositionOnLivePath& positionOnLivePath);


	// ---------------------------------- Public Data Members ----------------------------------

	/// The NavTag the PositionOnPath is allowed to move on.
	const NavTag* m_navTag;
};

} // namespace Kaim

#endif // Navigation_NavTagMovePositionOnPathPredicate_H
