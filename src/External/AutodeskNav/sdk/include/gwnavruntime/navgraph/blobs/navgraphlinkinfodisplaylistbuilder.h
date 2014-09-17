/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// Primary contact: MAMU - secondary contact: NOBODY
#ifndef Navigation_NavGraphLinkInfo_DisplayListBuilder_H
#define Navigation_NavGraphLinkInfo_DisplayListBuilder_H

#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"


namespace Kaim
{

class NavGraphVertexSpatializationBlob;
class DisplayShapeColor;
class VisualColor;

enum NavGraphLinkInfoDisplayListBuilderConfig
{
	NavGraphLinkInfo_Display_Spatialization,
	NavGraphLinkInfo_Display_LinkFromOutsideDetails
};

class NavGraphLinkInfoDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init() { InitConfig(m_config); }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Other; }

	static void InitConfig(DisplayListBuilderConfig& config);

private:
	void BuildSpatializationDisplayList(ScopedDisplayList* displayList, NavGraphVertexSpatializationBlob* spatialization, const DisplayShapeColor& color);
};

class NavGraphArrayLinkInfoDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init() { NavGraphLinkInfoDisplayListBuilder::InitConfig(m_config); m_navGraphLinkInfoDisplayListBuilder.Init(); }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_NavGraph; }

private:
	void BuildSpatializationDisplayList(ScopedDisplayList* displayList, NavGraphVertexSpatializationBlob* spatialization, VisualColor& color);
	NavGraphLinkInfoDisplayListBuilder m_navGraphLinkInfoDisplayListBuilder;
};

}

#endif