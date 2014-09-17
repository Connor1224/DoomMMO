/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: none
#ifndef KyRuntime_SpatializedCylinderDisplay_H
#define KyRuntime_SpatializedCylinderDisplay_H


#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "spatializedpointdisplay.h"

namespace Kaim
{

class SpatializedCylinderBlob;
class SpatializedCylinder;

enum SpatializedCylinderDisplayListBuilderConfigEnum
{
	SpatializedCylinderDisplay_Wireframe,
	SpatializedCylinderDisplay_Velocity,
	SpatializedCylinderDisplay_Spatialization
};

class SpatializedCylinderDisplayConfig
{
public:
	SpatializedCylinderDisplayConfig() { SetDefaults(); }

	void SetDefaults()
	{
		m_spatializedPointDisplayConfig.SetDefaults();
		m_displayAsWireframe = false;
		m_displaySpatialization = false;
		m_displayVelocity = false;
	}

	SpatializedPointDisplayConfig m_spatializedPointDisplayConfig;
	bool m_displayAsWireframe;
	bool m_displaySpatialization;
	bool m_displayVelocity;
};

class SpatializedCylinderDisplayParameters
{
public:
	SpatializedCylinderDisplayParameters() { SetDefaults(); }

	void SetDefaults()
	{
		m_cylinderColor = VisualColor::Magenta;
		m_cylinderLinesColor = VisualColor::Magenta;
		m_velocityColor = VisualColor::White;
		m_cylinderEdgeCount = 5;
	}

	SpatializedPointDisplayParameters m_spatializedPointDisplayParameters;
	VisualColor m_cylinderColor;
	VisualColor m_cylinderLinesColor;
	VisualColor m_velocityColor;
	KyUInt32 m_cylinderEdgeCount;
};

class SpatializedCylinderDisplayListBuilder : public IDisplayListBuilder
{
public:
	SpatializedCylinderDisplayListBuilder() {}

	virtual void Init();

private:
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize);

public:
	static void Display(
		ScopedDisplayList* displayList, const SpatializedCylinderBlob* spatializedCylinderBlob,
		const SpatializedCylinderDisplayConfig& displayConfig,
		const SpatializedCylinderDisplayParameters& parameters = SpatializedCylinderDisplayParameters());

	SpatializedCylinderDisplayParameters m_parameters;
};

} // namespace Kaim

#endif
