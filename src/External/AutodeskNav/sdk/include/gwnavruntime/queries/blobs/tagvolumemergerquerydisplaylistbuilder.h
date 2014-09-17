/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef Navigation_TagVolumeMergerQueryDisplayListBuilder_H
#define Navigation_TagVolumeMergerQueryDisplayListBuilder_H

#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/visualsystem/visualcolor.h"


namespace Kaim
{

class ITerrainTypeColorPalette;

enum TagVolumeMergerQueryDisplayListBuilderEnum
{
	TagVolumeMergerQueryDisplay_Wireframe,
	TagVolumeMergerQueryDisplay_Transparency,
	TagVolumeMergerQueryDisplay_Offset
};

class TagVolumeMergerQueryDisplayListBuilder: public IDisplayListBuilder
{
public:
	TagVolumeMergerQueryDisplayListBuilder()
	{}

	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Other; }
};

} // namespace Kaim

#endif // Navigation_TagVolumeMergerQueryDisplayListBuilder_H
