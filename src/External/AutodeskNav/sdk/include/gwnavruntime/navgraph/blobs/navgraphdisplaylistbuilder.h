/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// Primary contact: MAMU - secondary contact: NOBODY
#ifndef Navigation_NavGraph_DisplayListBuilder_H
#define Navigation_NavGraph_DisplayListBuilder_H

#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"


namespace Kaim
{

enum NavGraphDisplayListBuilderConfig
{
	NAVGRAPH_DISPLAY_EDGES_LINES,
	NAVGRAPH_DISPLAY_EDGES_ARROWS,
	NAVGRAPH_DISPLAY_EDGES_BOUNCING_ARROWS,
	NAVGRAPH_DISPLAY_GRAPH_NAME
};

class NavGraphDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init() { InitConfig(m_config); }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);

	static void InitConfig(DisplayListBuilderConfig& config)
	{
		config.AddBoolParam("Edges lines", false, NAVGRAPH_DISPLAY_EDGES_LINES);
		config.AddBoolParam("Edges arrows", true, NAVGRAPH_DISPLAY_EDGES_ARROWS);
		config.AddBoolParam("Edges bouncing arrows", false, NAVGRAPH_DISPLAY_EDGES_BOUNCING_ARROWS);
		config.AddBoolParam("Graph names", false, NAVGRAPH_DISPLAY_GRAPH_NAME);
	}
};

class NavGraphArrayDisplayListBuilder : public IDisplayListBuilder
{
public:
	virtual void Init() { NavGraphDisplayListBuilder::InitConfig(m_config); m_navGraphDisplayListBuilder.Init(); }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_NavGraph; }

	NavGraphDisplayListBuilder m_navGraphDisplayListBuilder;
};


}

#endif

