/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Contacts: LAPA
#ifndef Navigation_BoxObstacleDisplay_H
#define Navigation_BoxObstacleDisplay_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"


namespace Kaim
{

class BoxObstacleBlob;


enum BoxObstacleDisplayListBuilderEnum
{
	BoxObstacleDisplay_Wireframe,
	BoxObstacleDisplay_ForceBoxRender,
	BoxObstacleDisplay_Transform,
	BoxObstacleDisplay_BoxVelocities,
	BoxObstacleDisplay_SpatializedCylinders,
	BoxObstacleDisplay_SpatializedCylinderVelocities,
	BoxObstacleDisplay_Spatialization
};

class BoxObstacleDisplayListBuilder : public IDisplayListBuilder
{
public:
	BoxObstacleDisplayListBuilder()
		: m_boxColor(VisualColor::Green)
		, m_obstacleLinearVelocityColor(VisualColor::LightGreen)
		, m_obstacleAngularVelocityColor(VisualColor::LightCyan)
		, m_obstacleAngularSpeedColor(VisualColor::Cyan)
		, m_spatializedCylindersColor_Projected(VisualColor::Lime)
		, m_spatializedCylindersColor_NotProjected(VisualColor::Red)
		, m_spatializedCylindersVelocityColor(VisualColor::White)
		, m_spatializationLinkColor(VisualColor::LightYellow)
		, m_linearVelocityArrowWidth(0.05f)
		, m_angularVelocityArrowWidth(0.05f)
	{}

	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_BoxObstacle; }

	VisualColor m_boxColor;
	VisualColor m_obstacleLinearVelocityColor;
	VisualColor m_obstacleAngularVelocityColor;
	VisualColor m_obstacleAngularSpeedColor;
	VisualColor m_spatializedCylindersColor_Projected;
	VisualColor m_spatializedCylindersColor_NotProjected;
	VisualColor m_spatializedCylindersVelocityColor;
	VisualColor m_spatializationLinkColor;
	KyFloat32 m_linearVelocityArrowWidth;
	KyFloat32 m_angularVelocityArrowWidth;
};

}

#endif

