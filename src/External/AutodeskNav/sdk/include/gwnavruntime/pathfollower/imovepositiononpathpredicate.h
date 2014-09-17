/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_IMovePositionOnPathPredicate_H
#define Navigation_IMovePositionOnPathPredicate_H


namespace Kaim
{

class Bot;
class PositionOnLivePath;


/// Interface for predicate used to verify a PositionOnPath can be moved forward along the Path.
class IMovePositionOnPathPredicate
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	IMovePositionOnPathPredicate() {}
	virtual ~IMovePositionOnPathPredicate() {}


	// ---------------------------------- Pure Virtual Functions ----------------------------------

	virtual bool CanMoveForward(Bot* bot, PositionOnLivePath& positionOnLivePath) = 0;
	virtual bool CanMoveBackward(Bot* bot, PositionOnLivePath& positionOnLivePath) = 0;
};

} // namespace Kaim

#endif // Navigation_IMovePositionOnPathPredicate_H
