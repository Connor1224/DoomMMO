/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_TrajectoryDisplay_H
#define Navigation_TrajectoryDisplay_H

#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{

class TrajectoryDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Bot; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
};

} // namespace Kaim

#endif // Navigation_TrajectoryDisplay_H
