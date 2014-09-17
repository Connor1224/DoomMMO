/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_LivePathDisplayListBuilder_H
#define Navigation_LivePathDisplayListBuilder_H

#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{

class LivePath;
class LivePathBlob;
class LivePathDetailsBlob;
class PositionOnPathBlob;
class PathBlob;
class PathFinderQueryBlob;

enum LivePathDisplayConfig
{
	LivePathDisplay_path,
	LivePathDisplay_status
};

class LivePathDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Bot; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);

private:
	void DisplayPathFinderResult(ScopedDisplayList* displayList,
		KyUInt32 pathFinderResult, const Vec3f& pathFinderStart, const Vec3f* pathFinderDest);

	void DisplayPathfinderStatus(ScopedDisplayList* displayList,
		const PathFinderQueryBlob& pathFinderInfo, KyFloat32 botHeight, const Vec3f& botPosition);

	void DisplayPathAndLivePathBounds(ScopedDisplayList* displayList, KyUInt32 pathType, KyUInt32 pathValidityStatus, 
		const PositionOnPathBlob* backwardValidityBound, const PositionOnPathBlob* forwardValidityBound, const PathBlob* pathBlob);

	void DisplayPathEventList(ScopedDisplayList* displayList, const LivePathDetailsBlob* livePathDetailsBlob);

	void PushLivePathBoundPosOnPath(ScopedDisplayList* displayList, const VisualColor& color, char* posOnPathBlob);

private:
	KyFloat32 m_pathBoundRadius;
	KyFloat32 m_pathEventMastHeight;
	KyFloat32 m_pathEventFlagHeight;
	KyFloat32 m_pathEventRadius;
};

} // namespace Kaim

#endif
