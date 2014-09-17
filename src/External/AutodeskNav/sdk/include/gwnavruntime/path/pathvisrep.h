/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_PathVisRep_H
#define Navigation_PathVisRep_H

#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{

class Path;


class PathDisplayListBuilder : public IDisplayListBuilder
{
public:

	enum DrawingMode
	{
		OneColor,
		TwoColor,
		ThreeColors
	};

	enum PathDisplayListBuilderConfig
	{
		PathDisplay_AltitudeOffset
	};

	PathDisplayListBuilder()
		: m_nodesColor(VisualColor::Cyan)
		, m_nodesAltitudeOffset(0.5f)
		, m_nodesHalfWidth(0.1f)
		, m_edgesColor(VisualColor::LawnGreen)
		, m_edgesAltitudeOffset(0.5f)
		, m_mode(OneColor)
		, m_firstPathSplit(KY_NULL)
		, m_secondPathSplit(KY_NULL)
	{}

	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Bot; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);

	VisualColor m_nodesColor;
	KyFloat32 m_nodesAltitudeOffset;
	KyFloat32 m_nodesHalfWidth;
	VisualColor m_edgesColor;
	KyFloat32 m_edgesAltitudeOffset;
	DrawingMode m_mode;
	const Vec3f* m_firstPathSplit;
	VisualColor m_secondEdgesColor;
	const Vec3f* m_secondPathSplit;
	VisualColor m_thirdEdgesColor;
};

} // namespace Kaim

#endif // Navigation_PathVisRep_H
