/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: BRGR
#ifndef KyRuntime_CylinderObstacleDisplay_H
#define KyRuntime_CylinderObstacleDisplay_H


#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{

class CylinderObstacleBlob;
class CylinderObstacle;

enum CylinderObstacleDisplayListBuilderConfigEnum
{
	CylinderObstacleDisplay_Wireframe,
	CylinderObstacleDisplay_ForceCylinderRender,
	CylinderObstacleDisplay_Spatialization,
	CylinderObstacleDisplay_Velocity
};

class CylinderObstacleDisplayListBuilder : public IDisplayListBuilder
{
public:
	CylinderObstacleDisplayListBuilder() {}
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_CylinderObstacle; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize);
};

} // namespace Kaim

#endif // Navigation_CylinderObstacleDisplay_H
