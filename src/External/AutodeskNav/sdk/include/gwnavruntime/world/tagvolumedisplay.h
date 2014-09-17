/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_TagVolumeDisplayListBuilder_H
#define Navigation_TagVolumeDisplayListBuilder_H

#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/navdata/terraintypecolorpalette.h"


namespace Kaim
{

class TagVolumeBlob;


enum TagVolumeDisplayFlags
{
	TagVolumeDisplay_Contour,
	TagVolumeDisplay_AABB,
	TagVolumeDisplay_ObstacleType
};

class TagVolumeDisplayListBuilder: public IDisplayListBuilder
{
public:
	TagVolumeDisplayListBuilder()
		: m_aabbColor_Projected(VisualColor::Lime)
		, m_aabbColor_NotProjected(VisualColor::Red)
		, m_contourColor(VisualColor::Cyan)
	{}

	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_TagVolume; }

	VisualColor m_aabbColor_Projected;
	VisualColor m_aabbColor_NotProjected;
	VisualColor m_contourColor;
	TerrainTypeColorPalette m_terrainTypePalette;
};

enum TagVolumeContextDisplayFlags
{
	TagVolumeContextDisplay_WorldStatus,
	TagVolumeContextDisplay_IntegrationStatus
};

class TagVolumeContextDisplayListBuilder: public IDisplayListBuilder
{
public:
	TagVolumeContextDisplayListBuilder() {}

	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_TagVolume; }
};


} // namespace Kaim

#endif // Navigation_TagVolumeDisplayListBuilder_H
