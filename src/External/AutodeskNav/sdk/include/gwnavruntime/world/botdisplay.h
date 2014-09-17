/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_BotDisplay_H
#define Navigation_BotDisplay_H

#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{

class Bot;
class BotBlob;


/// Defines the colors used to render the Bot accordingly to its internal
/// statuses. Possible values are:
/// - Red     =>  BotColor_PathNotFound         =>  No Path has been found for this Bot.
/// - Orange  =>  BotColor_ComputingPath        =>  Path is under computation.
/// - Lime    =>  BotColor_NoPathSet            =>  No Path is set nor under computation (the Bot idles).
/// - Yellow  =>  BotColor_FullPathFollower     =>  The Bot computes all in the PathFollower.
/// - Blue    =>  BotColor_CustomFollowing      =>  The at least one component of path following is done by the client.
typedef VisualColor BotColor;

// - path status
static const BotColor BotColor_PathNotFound        = VisualColor::Red;
static const BotColor BotColor_ComputingPath       = VisualColor::Orange;
static const BotColor BotColor_NoPathSet           = VisualColor::Lime;
// - pathfollower status
static const BotColor BotColor_FullPathFollower    = VisualColor::Yellow;
static const BotColor BotColor_CustomFollowing     = VisualColor::Blue;


enum BotDisplayListBuilderConfig
{
	// Generic information
	BotDisplay_Status,
	BotDisplay_Wireframe,

	// Spatialization
	BotDisplay_Spatialization,

	// Path & PathFinding
	BotDisplay_TargetOnPath,

	// misc
	BotDisplay_Velocity
};

class BotDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Bot; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);

private:
	void DisplayStatus(ScopedDisplayList* displayList, const BotBlob* botBlob, bool displayFullStatus);
	void DisplayTargetOnPath(ScopedDisplayList* displayList, const BotBlob* botBlob);

public:
	KyFloat32 m_pathFlagOffset;
	KyFloat32 m_targetOnPathRenderRadius;
};

} // namespace Kaim

#endif // Navigation_BotDisplay_H
